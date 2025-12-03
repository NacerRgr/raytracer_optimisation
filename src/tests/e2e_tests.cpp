#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <numeric>
#include "../lodepng/lodepng.h"

// These macros are needed to turn the RAYTRACER_EXECUTABLE definition from CMake into a string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Helper function to run the raytracer executable
void runRaytracer(const std::string& scenePath, const std::string& outputPath) {
    // Construct the command to run the main executable
    // The path to the executable is passed from CMake during compilation
    std::string executable_path = TOSTRING(RAYTRACER_EXECUTABLE);
    std::string command = executable_path + " " + scenePath + " " + outputPath;
    std::cout << "Executing command: " << command << std::endl;
    
    int result = system(command.c_str());
    ASSERT_EQ(result, 0) << "Raytracer executable failed to run for scene: " << scenePath;
}

// Helper function to load a PNG file into a raw RGBA buffer
bool loadImage(const std::string& filename, std::vector<unsigned char>& image, unsigned& width, unsigned& height) {
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error) {
        std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }
    return true;
}

// Helper function to calculate Root Mean Square Error (RMSE) between two images
double calculate_rmse(const std::vector<unsigned char>& img1, unsigned w1, unsigned h1,
                      const std::vector<unsigned char>& img2, unsigned w2, unsigned h2) {
    // Use EXPECT_EQ in functions that return a value, as ASSERT_EQ would cause a compile error.
    EXPECT_EQ(w1, w2);
    EXPECT_EQ(h1, h2);
    if (w1 != w2 || h1 != h2) {
        return -1.0; // Return an error indicator
    }

    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < img1.size(); i += 4) {
        // The raw buffer is just a sequence of R,G,B,A bytes.
        // We compare them directly.
        double diff_r = (double)img1[i + 0] - (double)img2[i + 0];
        double diff_g = (double)img1[i + 1] - (double)img2[i + 1];
        double diff_b = (double)img1[i + 2] - (double)img2[i + 2];
        // Alpha channel (i+3) is ignored for this comparison.

        sum_sq_diff += diff_r * diff_r;
        sum_sq_diff += diff_g * diff_g;
        sum_sq_diff += diff_b * diff_b;
    }

    double num_pixels = w1 * h1;
    double mean_sq_err = sum_sq_diff / (num_pixels * 3);
    return std::sqrt(mean_sq_err);
}


TEST(E2ETests, RegularScene_AllObjects) {
    const std::string source_dir = TOSTRING(PROJECT_SOURCE_DIR);
    const std::string scene_path = source_dir + "/scenes/all.json";
    const std::string output_path = "test_all_output.png";
    const std::string golden_path = source_dir + "/tests/golden/all_golden.png";

    // 1. Run the raytracer to generate the output image
    runRaytracer(scene_path, output_path);

    // 2. Load the generated image and the golden image
    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h)) << "Failed to load generated image: " << output_path;

    std::vector<unsigned char> golden_image;
    unsigned gold_w, gold_h;
    ASSERT_TRUE(loadImage(golden_path, golden_image, gold_w, gold_h)) << "Failed to load golden image: " << golden_path;

    // 3. Compare the images
    double rmse = calculate_rmse(generated_image, gen_w, gen_h, golden_image, gold_w, gold_h);
    
    // Print the metric
    std::cout << "RMSE for RegularScene_AllObjects: " << rmse << std::endl;

    // Allow for a small difference due to floating point variations
    const double threshold = 1.0; 
    EXPECT_LT(rmse, threshold) << "Image difference (RMSE) exceeds threshold.";
}

TEST(E2ETests, EdgeCase_EmptyScene) {
    const std::string source_dir = TOSTRING(PROJECT_SOURCE_DIR);
    const std::string scene_path = source_dir + "/scenes/empty.json";
    const std::string output_path = "test_empty_output.png";
    const std::string golden_path = source_dir + "/tests/golden/empty_golden.png";

    runRaytracer(scene_path, output_path);

    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h));

    std::vector<unsigned char> golden_image;
    unsigned gold_w, gold_h;
    ASSERT_TRUE(loadImage(golden_path, golden_image, gold_w, gold_h));

    double rmse = calculate_rmse(generated_image, gen_w, gen_h, golden_image, gold_w, gold_h);
    std::cout << "RMSE for EdgeCase_EmptyScene: " << rmse << std::endl;
    
    const double threshold = 1.0;
    EXPECT_LT(rmse, threshold);
}

TEST(E2ETests, DeliberateFailure_Demonstration) {
    const std::string source_dir = TOSTRING(PROJECT_SOURCE_DIR);
    const std::string scene_path = source_dir + "/scenes/all.json";
    const std::string output_path = "test_failure_output.png";
    // We compare the complex scene output with the "empty" golden image to force a failure.
    const std::string wrong_golden_path = source_dir + "/tests/golden/empty_golden.png";

    runRaytracer(scene_path, output_path);

    std::vector<unsigned char> generated_image;
    unsigned gen_w, gen_h;
    ASSERT_TRUE(loadImage(output_path, generated_image, gen_w, gen_h));

    std::vector<unsigned char> wrong_golden_image;
    unsigned gold_w, gold_h;
    ASSERT_TRUE(loadImage(wrong_golden_path, wrong_golden_image, gold_w, gold_h));

    double rmse = calculate_rmse(generated_image, gen_w, gen_h, wrong_golden_image, gold_w, gold_h);
    std::cout << "RMSE for DeliberateFailure_Demonstration: " << rmse << std::endl;

    const double threshold = 1.0;
    // This test should fail the comparison, so we expect the error to be HIGH.
    EXPECT_GT(rmse, threshold) << "This test is designed to fail, but the RMSE was unexpectedly low.";
}