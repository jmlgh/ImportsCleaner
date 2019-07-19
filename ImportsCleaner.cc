#include <iostream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <algorithm>
#include <fstream>


namespace fs = std::experimental::filesystem;


std::vector<fs::path> get_path_contents(const fs::path& path, const std::string& extension);
std::vector<std::string> read_requirements(const fs::path& requirements_path);
bool ends_with(const std::string& fullstring, const std::string& extension);
bool starts_with(const std::string& fullstring, const std::string& term);


int main(int argc, char* argv[])
{
  fs::path path{};
  std::string extension{};
  argc > 1 ? path = argv[1] : path = fs::current_path();
  argc > 2 ? extension = argv[2] : extension = ".py"; // looking for py files by default

  if (!fs::exists(path)) {
    std::cerr << "Specified path does not exist" << std::endl;
    return -1;
  }

  if (!fs::is_directory(path)) {
    std::cerr << "Specified path is not a directory" << std::endl;
    return -1;
  }

  auto dir_contents{get_path_contents(path, extension)};
  auto found = std::find_if(begin(dir_contents), end(dir_contents), [](const fs::path& content){return (0 == content.filename().string().compare("requirements.txt"));});
  if (found != end(dir_contents)) {
    // requirements file was found
    auto requirements = read_requirements(*found);
    std::for_each(begin(requirements), end(requirements), [](const std::string& req){std::cout << req << std::endl;});
  }
  else
    // requirements file was not found
    std::cout << "Requirements file not found" << std::endl;

  return 0;
}

std::vector<fs::path> get_path_contents(const fs::path& path, const std::string& extension)
{
  std::vector<fs::path> dir_contents{};
  std::string requirements = "requirements.txt";
  for (const auto& content : fs::directory_iterator(path)) {
    if (ends_with(content.path().filename().string(), extension))
      dir_contents.push_back(content);
    if (content.path().filename().string().compare(requirements) == 0)
      dir_contents.push_back(content);
  }

  return dir_contents;
}


std::vector<std::string> read_requirements(const fs::path& requirements_path)
{
  std::vector<std::string> requirements{};
  std::ifstream file(requirements_path);
  std::string line{};
  std::string import{"import"};
  std::string from_import{"from"};

  if (!file.is_open()) {
   std::cerr << "Failed to open the requirements.txt file" << std::endl;
  } else {
    while(getline(file, line)) {
      int equals_index = line.find("==");
      std::string import = line.substr(0, equals_index);
      requirements.push_back(import);
    }
  }

  return requirements;
}


bool starts_with(const std::string& fullstring, const std::string& term)
{
  return fullstring.find(term) == 0;
}


bool ends_with(const std::string& fullstring, const std::string& extension)
{
  if (fullstring.length() >= extension.length()) {
    return (0 == fullstring.compare(fullstring.length() - extension.length(), extension.length(), extension));
  } else {
    return false;
  }
}
