#include "Magick++/Functions.h"
#include "Magick++/Geometry.h"
#include "Magick++/Include.h"
#include <memory>
#include <ostream>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <Magick++.h>


Magick::Image mask;

std::vector<std::string> take_top_n_of_vector(std::shared_ptr<std::vector<std::string>> input_vec, int n) {
  std::vector<std::string> output;
  for (int i = 0; i <= n && i <= input_vec->size(); ++i) {
    output.push_back(input_vec->back());
    input_vec->pop_back();
  }
  return output;
}

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

  std::shared_ptr<std::vector<std::string>> files_in_directory = std::make_shared<std::vector<std::string>>();
  for (const auto & entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      files_in_directory->push_back(entry.path());
    }
  }

while (files_in_directory->size() != 0) {
  auto files = take_top_n_of_vector(files_in_directory, 10);
  

  std::vector<std::thread> threads;
  for (auto file : files) {
    threads.push_back(std::move(std::thread(do_the_thing, file)));
  }
  for (auto &t : threads) {
    t.join();
  }
  }
}
