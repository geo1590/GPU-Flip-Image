#include <Exceptions.h>
#include <ImageIO.h>
#include <ImagesCPU.h>
#include <ImagesNPP.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <cuda_runtime.h>
#include <npp.h>
#include <helper_cuda.h>
#include <helper_string.h>
#include <cmath>
#include <vector>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <nppi.h>
#include <filesystem>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

void mirrorFlipNPP(const Npp8u*, Npp8u*, int, int, bool);
int convert(const std::string&, const std::string&, bool);
std::string to_out_path(const std::string&);
int process_files();
