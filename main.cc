#include "Magick++/Functions.h"
#include "Magick++/Geometry.h"
#include "Magick++/Include.h"
#include <ostream>
#include <pstl/glue_execution_defs.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <execution>
#include <Magick++.h>


Magick::Image mask;

void show_usage_of_app(void) {
  std::cout << "USAGE: ./betterButtonMaker {PATH TO DIRECTORY} {PATH TO MASK}" << std::endl;
}

void do_the_thing(std::string e){

    Magick::Image copy_mask(mask);

    std::cout << e << std::endl;
    Magick::Image i;
    i.read(e);
    copy_mask.resize(i.size());
    i.magick("PNG");
    i.composite(copy_mask, Magick::GravityType::CenterGravity, Magick::OverCompositeOp);
    int height = copy_mask.size().height();
    i.crop(Magick::Geometry(height, height, (i.size().width() - copy_mask.size().width()) / 2, 0));
    i.write(e + "_masked.png");
} 


int main(int argc, char* argv[]) {


  if(argc != 3) {
    show_usage_of_app();
    return -10;
  }

  Magick::InitializeMagick(*argv);

  mask.magick("PNG");
  mask.read(argv[2]);


  std::string path = argv[1];

  std::vector<std::string> files_in_directory;
  for (const auto & entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      files_in_directory.push_back(entry.path());
    }
  }

//  for (auto e : files_in_directory) {
//    Magick::Image copy_mask(mask);
//
//    std::cout << e << std::endl;
//    Magick::Image i;
//    i.read(e);
//    copy_mask.resize(i.size());
//    i.magick("PNG");
//    i.composite(copy_mask, Magick::GravityType::CenterGravity, Magick::OverCompositeOp);
//    int height = copy_mask.size().height();
//    i.crop(Magick::Geometry(height, height, (i.size().width() - copy_mask.size().width()) / 2, 0));
//    i.write(e + "_masked.png");
//  }

std::for_each(
    std::execution::par,
    files_in_directory.begin(),
    files_in_directory.end(),
    do_the_thing);
}