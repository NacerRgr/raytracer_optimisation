#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include "../lodepng/lodepng.h"

// These macros are needed to turn the RAYTRACER_EXECUTABLE definition from CMake into a string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Helper function to run the raytracer executable and return execution time
double runRaytracer(const std::string& scenePath, const std::string& outputPath, bool expectSuccess = true) {
    std::string executable_path = TOSTRING(RAYTRACER_EXECUTABLE);
    // Remove quotes if present
    if (!executable_path.empty() && executable_path.front() == '"' && executable_path.back() == '"') {
        executable_path = executable_path.substr(1, executable_path.length() - 2);
    }
    
    std::string command = executable_path + " " + scenePath + " " + outputPath;
    std::cout << "Executing command: " << command << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    int result = system(command.c_str());
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> elapsed = end - start;
    double execution_time = elapsed.count();
    
    if (expectSuccess) {
        EXPECT_EQ(result, 0) << "Raytracer executable failed to run for scene: " << scenePath;
    }
    
    return execution_time;
}

// Helper function to load a PNG file
bool loadImage(const std::string& filename, std::vector<unsigned char>& image, unsigned& width, unsigned& height) {
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }
    file.close();
    
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error) {
        std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }
    return true;
}

// Helper function to calculate RMSE between two images
double calculate_rmse(const std::vector<unsigned char>& img1, unsigned w1, unsigned h1,
                      const std::vector<unsigned char>& img2, unsigned w2, unsigned h2) {
    EXPECT_EQ(w1, w2);
    EXPECT_EQ(h1, h2);
    if (w1 != w2 || h1 != h2) {
        return -1.0;
    }

    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < img1.size(); i += 4) {
        double diff_r = (double)img1[i + 0] - (double)img2[i + 0];
        double diff_g = (double)img1[i + 1] - (double)img2[i + 1];
        double diff_b = (double)img1[i + 2] - (double)img2[i + 2];

        sum_sq_diff += diff_r * diff_r;
        sum_sq_diff += diff_g * diff_g;
        sum_sq_diff += diff_b * diff_b;
    }

    double num_pixels = w1 * h1;
    double mean_sq_err = sum_sq_diff / (num_pixels * 3);
    return std::sqrt(mean_sq_err);
}

// ============================================================================
// TEST 1 : Cas d'utilisation régulier (3 points)
// ============================================================================
TEST(RaytracerE2E, RegularUseCase_TwoSpheres) {
    const std::string scene_path = "/app/scenes/two-spheres-on-plane.json";
    const std::string output_path = "test_regular.png";
    const std::string golden_path = "/app/src/tests/golden/reference.png";

    std::cout << "\n=== TEST 1 : Cas d'utilisation régulier ===" << std::endl;

    // Exécuter le raytracer
    double exec_time = runRaytracer(scene_path, output_path);
    std::cout << "⏱️  Temps d'exécution : " << exec_time << " secondes" << std::endl;

    // Charger l'image générée
    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h)) 
        << "Failed to load generated image";
    
    std::cout << "✓ Image générée : " << gen_w << "x" << gen_h << " pixels" << std::endl;

    // Vérifier dimensions
    EXPECT_GT(gen_w, 0);
    EXPECT_GT(gen_h, 0);

    // Vérifier pixels colorés
    bool has_non_black_pixels = false;
    for (size_t i = 0; i < generated_image.size(); i += 4) {
        if (generated_image[i] > 0 || generated_image[i+1] > 0 || generated_image[i+2] > 0) {
            has_non_black_pixels = true;
            break;
        }
    }
    EXPECT_TRUE(has_non_black_pixels);
    std::cout << "✓ L'image contient des pixels colorés" << std::endl;

    // Comparer avec golden image
    std::vector<unsigned char> golden_image;
    unsigned gold_w, gold_h;
    ASSERT_TRUE(loadImage(golden_path, golden_image, gold_w, gold_h));

    double rmse = calculate_rmse(generated_image, gen_w, gen_h, golden_image, gold_w, gold_h);
    std::cout << "✓ RMSE : " << rmse << std::endl;
    
    EXPECT_LT(rmse, 1.0);
    std::cout << "=== TEST 1 RÉUSSI ===" << std::endl;
}

// ============================================================================
// TEST 2 : Cas limite - Scène vide (3 points)
// ============================================================================
TEST(RaytracerE2E, EdgeCase_EmptyScene) {
    const std::string scene_path = "/app/scenes/empty.json";
    const std::string output_path = "test_empty.png";
    const std::string golden_path = "/app/src/tests/golden/empty_golden.png";

    std::cout << "\n=== TEST 2 : Cas limite - Scène vide ===" << std::endl;

    // Exécuter le raytracer
    double exec_time = runRaytracer(scene_path, output_path);
    std::cout << "⏱️  Temps d'exécution : " << exec_time << " secondes" << std::endl;

    // Charger l'image générée
    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h));
    
    std::cout << "✓ Image générée : " << gen_w << "x" << gen_h << " pixels" << std::endl;

    // Pour une scène vide, l'image devrait être principalement noire
    int black_pixel_count = 0;
    int total_pixels = gen_w * gen_h;
    
    for (size_t i = 0; i < generated_image.size(); i += 4) {
        if (generated_image[i] == 0 && generated_image[i+1] == 0 && generated_image[i+2] == 0) {
            black_pixel_count++;
        }
    }
    
    double black_percentage = (double)black_pixel_count / total_pixels * 100.0;
    std::cout << "✓ Pixels noirs : " << black_percentage << "%" << std::endl;
    
    // Au moins 90% de l'image devrait être noire pour une scène vide
    EXPECT_GT(black_percentage, 90.0);

    // Comparer avec golden image si elle existe
    std::vector<unsigned char> golden_image;
    unsigned gold_w, gold_h;
    if (loadImage(golden_path, golden_image, gold_w, gold_h)) {
        double rmse = calculate_rmse(generated_image, gen_w, gen_h, golden_image, gold_w, gold_h);
        std::cout << "✓ RMSE : " << rmse << std::endl;
        EXPECT_LT(rmse, 1.0);
    }
    
    std::cout << "=== TEST 2 RÉUSSI ===" << std::endl;
}

// ============================================================================
// TEST 3 : Scénario d'échec - Fichier invalide (2 points)
// ============================================================================
TEST(RaytracerE2E, FailureScenario_InvalidScene) {
    const std::string scene_path = "/app/scenes/nonexistent_file.json";
    const std::string output_path = "test_failure.png";

    std::cout << "\n=== TEST 3 : Scénario d'échec - Fichier invalide ===" << std::endl;

    // Vérifier que le fichier n'existe pas
    std::ifstream file(scene_path);
    EXPECT_FALSE(file.good()) << "Le fichier de test ne devrait pas exister";
    
    std::cout << "✓ Fichier de test inexistant confirmé" << std::endl;

    // Exécuter le raytracer - on s'attend à un échec
    std::string executable_path = TOSTRING(RAYTRACER_EXECUTABLE);
    if (!executable_path.empty() && executable_path.front() == '"' && executable_path.back() == '"') {
        executable_path = executable_path.substr(1, executable_path.length() - 2);
    }
    
    std::string command = executable_path + " " + scene_path + " " + output_path + " 2>/dev/null";
    int result = system(command.c_str());
    
    // Le raytracer devrait retourner un code d'erreur (non-zéro)
    EXPECT_NE(result, 0) << "Le raytracer devrait échouer avec un fichier invalide";
    
    std::cout << "✓ Le raytracer a correctement détecté l'erreur (code retour : " << result << ")" << std::endl;
    std::cout << "=== TEST 3 RÉUSSI : Gestion d'erreur validée ===" << std::endl;
}

// ============================================================================
// TEST 4 : Métriques de performance (2 points)
// ============================================================================
TEST(RaytracerE2E, PerformanceMetrics_ComplexScene) {
    const std::string scene_path = "/app/scenes/iso-sphere-on-plane.json";
    const std::string output_path = "test_performance.png";

    std::cout << "\n=== TEST 4 : Métriques de performance - Scène complexe ===" << std::endl;

    // Exécuter le raytracer et mesurer le temps
    auto start = std::chrono::high_resolution_clock::now();
    double exec_time = runRaytracer(scene_path, output_path);
    auto end = std::chrono::high_resolution_clock::now();
    
    // Charger l'image
    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h));

    // Calculer les métriques
    int total_pixels = gen_w * gen_h;
    double pixels_per_second = total_pixels / exec_time;
    
    // Calculer la moyenne des valeurs de pixels (luminosité moyenne)
    long long sum_r = 0, sum_g = 0, sum_b = 0;
    for (size_t i = 0; i < generated_image.size(); i += 4) {
        sum_r += generated_image[i];
        sum_g += generated_image[i+1];
        sum_b += generated_image[i+2];
    }
    double avg_r = (double)sum_r / total_pixels;
    double avg_g = (double)sum_g / total_pixels;
    double avg_b = (double)sum_b / total_pixels;

    // Afficher les métriques
    std::cout << "\n📊 MÉTRIQUES DE PERFORMANCE :" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "Temps de rendu         : " << exec_time << " secondes" << std::endl;
    std::cout << "Résolution             : " << gen_w << " x " << gen_h << " pixels" << std::endl;
    std::cout << "Total de pixels         : " << total_pixels << std::endl;
    std::cout << "Pixels par seconde      : " << (int)pixels_per_second << std::endl;
    std::cout << "Luminosité moyenne (R)  : " << (int)avg_r << " / 255" << std::endl;
    std::cout << "Luminosité moyenne (G)  : " << (int)avg_g << " / 255" << std::endl;
    std::cout << "Luminosité moyenne (B)  : " << (int)avg_b << " / 255" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;

    // Vérifications de base
    EXPECT_GT(exec_time, 0.0);
    EXPECT_GT(total_pixels, 0);
    
    std::cout << "=== TEST 4 RÉUSSI : Métriques affichées ===" << std::endl;
}