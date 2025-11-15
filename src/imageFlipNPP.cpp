
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma warning(disable : 4819)
#endif

# include "imageFlipNPP.h"

namespace fs = std::filesystem;

// This function will:
//     -- Allocate device memory.
//     -- Copy the image data from the host to the device.
//     -- Call the CUDA NPP library function to flip the image.
//     -- Copy the resulting image data from the device to the host.
//     -- Free the device memory.
void mirrorFlipNPP(
    const Npp8u* h_src,  // host source image
    Npp8u*       h_dst,  // host dest image
    int          width,
    int          height,
    bool         horizontalFlip // true = left-right, false = top-bottom
)
{
    const int channels = 3;
    const int srcStep = width * channels * sizeof(Npp8u);
    const int dstStep = width * channels * sizeof(Npp8u);

    Npp8u *d_src = nullptr, *d_dst = nullptr;

    // Allocate device memory
    NPP_CHECK_CUDA(cudaMalloc(&d_src, srcStep * height));
    NPP_CHECK_CUDA(cudaMalloc(&d_dst, dstStep * height));

    // Copy input image to device
    NPP_CHECK_CUDA(cudaMemcpy(d_src, h_src, srcStep * height, cudaMemcpyHostToDevice));

    // Define image size for NPP
    NppiSize oSize;
    oSize.width  = width;
    oSize.height = height;

    // Choose flip direction
    NppiAxis flipAxis = horizontalFlip ? NPP_HORIZONTAL_AXIS : NPP_VERTICAL_AXIS;

    // Call NPP mirror for 8u_C3
    NPP_CHECK_NPP(nppiMirror_8u_C3R(
        d_src, srcStep,
        d_dst, dstStep,
        oSize,
        flipAxis
    ));

    // Copy result back to host
    NPP_CHECK_CUDA(cudaMemcpy(h_dst, d_dst, dstStep * height, cudaMemcpyDeviceToHost));

    // Cleanup
    cudaFree(d_src);
    cudaFree(d_dst);
}

// This function will read one input JPG input file at a time, convert it by horizontally flipping the image,
// write out the resulting image to a file. 
int convert(const std::string& inputPath, const std::string& outputPath, bool horizontalFlip)
{
    int width = 0, height = 0, channelsInFile = 0;
    const int desiredChannels = 3; // Force 8u_C3

    // Load PNG as 8-bit RGB
    unsigned char* h_src = stbi_load(
        inputPath.c_str(),
        &width,
        &height,
        &channelsInFile,
        desiredChannels
    );

    if (!h_src)
    {
        std::cerr << "Failed to load image: " << inputPath << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loaded " << inputPath
              << " (w=" << width << ", h=" << height
              << ", channels=" << desiredChannels << ")\n";

    size_t imgSize = static_cast<size_t>(width) * height * desiredChannels;
    unsigned char* h_dst = (unsigned char*)malloc(imgSize);
    if (!h_dst)
    {
        std::cerr << "Failed to allocate host memory for destination image.\n";
        stbi_image_free(h_src);
        return EXIT_FAILURE;
    }

    // Run NPP mirror flip
    mirrorFlipNPP(
        reinterpret_cast<Npp8u*>(h_src),
        reinterpret_cast<Npp8u*>(h_dst),
        width,
        height,
        horizontalFlip
    );

    // Write out PNG (stride = width * channels)
    int stride = width * desiredChannels;
    if (!stbi_write_png(outputPath.c_str(), width, height, desiredChannels, h_dst, stride))
    {
        std::cerr << "Failed to write output image: " << outputPath << std::endl;
        stbi_image_free(h_src);
        free(h_dst);
        return EXIT_FAILURE;
    }

    std::cout << "Wrote flipped image to " << outputPath << std::endl;

    stbi_image_free(h_src);
    free(h_dst);

    return EXIT_SUCCESS;
}

// This function will take the input file & path and create a output file & path. The /data directory contains
// the input JPG files and /out_data directory containts the output JPG files.
std::string to_out_path(const std::string& inPath)
{
    // Find the last '/'
    size_t pos = inPath.find_last_of('/');
    if (pos == std::string::npos) {
        // No directory; return unchanged
        return std::string("./out_data/") + inPath;
    }

    // Extract just the filename
    std::string filename = inPath.substr(pos + 1);

    // Build new path
    return std::string("./out_data/") + filename;
}


// This function will get all JPG files in /data and convert it by horizontally flipping the image.
// It will save the resulting files in /out_data directory.
int process_files() {
    std::string spath = "./data";   // source directory
    std::string dpath = "./out_data";   // destination directory
    std::vector<std::string> jpgFiles;
    bool horizontalFlip = true;

    try {
        for (const auto& entry : fs::directory_iterator(spath)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                // case-insensitive match for .jpg
                if (ext == ".jpg" || ext == ".JPG") {
                    jpgFiles.push_back(entry.path().string());
                }
            }
        }

        // Print results
        for (const auto& file : jpgFiles) {
	    std::string out_file = to_out_path(file);
            convert(file, out_file, horizontalFlip);
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}


// No arguments are needed.
int main(int argc, char** argv)
{
    process_files();
}

