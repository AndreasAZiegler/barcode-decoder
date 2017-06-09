#include <fstream>
#include "barcode_localization.h"

#include <mgl2/mgl.h>

int main(int argc, char** argv) {
  // Parameters
  int minLineLength = 30;
  int support_candidates_threshold = 7;
  int delta = 125;
  int maxLengthToLineLengthRatio = 8;
  int minLengthToLineLengthRatio = 2;
  int inSegmentXDistance = 300;
  int inSegmentYDistance = 100;

  if(argc != 2) {
    std::cout << "Usage: display_image ImageToLoadAndDisplay" << std::endl;
    return(-1);
  }

	// Fetch all image paths
	std::vector<cv::String> imageFilenames;
	cv::String imageFolder = argv[1];
	cv::glob(imageFolder, imageFilenames);
	std::cout << "Found " << imageFilenames.size() << " files in " << imageFolder << std::endl;

	std::ofstream stream("result.txt");

	int imageFilenames_size = imageFilenames.size();
	for(unsigned int i = 0; i < imageFilenames_size; i++) {
		std::cout << "Processing image: " << i << " out of " << imageFilenames_size << " images" << std::endl;

		cv::Mat image_color;
		image_color = cv::imread(imageFilenames[i], cv::IMREAD_COLOR);
		//cv::resize(image, image, cv::Size(1080, 960));

		if(!image_color.data) {
			std::cout << "No image data" << std::endl;
			return(-1);
		}

		std::vector<std::string> barcodes = locateBarcode(image_color,
																											minLineLength,
																											support_candidates_threshold,
																											delta,
																											maxLengthToLineLengthRatio,
																											minLengthToLineLengthRatio,
																											inSegmentXDistance,
																											inSegmentYDistance);

		bool firstMessageFlag = false;
		for(const std::string &str : barcodes) {
			if(0 < str.size()) {
				if(false == firstMessageFlag) {
					stream << std::to_string(i);
					firstMessageFlag = true;
				}
				stream << ";" << str;
			}
		}
		if(true == firstMessageFlag) {
			stream << "\n\n" << std::endl;
		}
	}

	/*
	for(auto& kv : imgNrBarcodes) {
		if(0 < kv.second.size()) {
			stream << std::to_string(kv.first) <<  ";";
			// Add '\n' character  ^^^^
			std::vector<std::string>::iterator setIter;
			for(setIter = kv.second.begin(); setIter != kv.second.end(); ++setIter) {
				if(0 < setIter->size()) {
					stream << ";" << *setIter<< std::endl;
				}
			}
			stream << "\n\n" << std::endl;
		}
	}
	*/
	stream.close();

	return(0);
}
