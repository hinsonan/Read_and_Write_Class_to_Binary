#include <iostream>
#include <vector>
#include <fstream>

size_t size_of_model;
std::vector<size_t> feature_sizes;
// This will be the class written to binary
class Car{
    public:
    std::string model;
    int num_of_wheels;
    std::vector<std::string> list_of_features;
    Car(std::string model, int num_of_wheels){
        this->model = model;
        this->num_of_wheels = num_of_wheels;
    }
    Car(){
        this->model = "";
        this->num_of_wheels = 0;
    }

    void print_car_details(){
        std::cout << this->model << std::endl;
        std::cout << this->num_of_wheels << std::endl;
        for(auto feature : this->list_of_features){
            std::cout<< feature << std::endl;
        }
    }
};
// writes the Car objects POD(plain old data) to a binary file
void write_to_binary_file(Car& car){
    std::fstream ofile("cars.bin", std::ios::binary | std::ios::out);
    // write the model to the file. you need the text and the size of the string
    ofile.write(car.model.c_str(), car.model.size());
    // keep track of model size for later in the program
    size_of_model = car.model.size();
    // write out the number of wheels. you need to tell it the size of an int
    ofile.write((char*)&car.num_of_wheels, sizeof(car.num_of_wheels));
    // loop through the strings in list of features
    for (std::string feature : car.list_of_features) {
        // keep track of the length of the strings in a vector
        feature_sizes.push_back(feature.size());
        // write the string the binary
        ofile.write(feature.c_str(), feature.size());
    }
    ofile.close();
}

Car read_bin_of_cars(std::string bin_file_path){
    Car new_car;
    std::fstream infile(bin_file_path, std::ios::binary | std::ios::in);
    std::string model;
    // Since a string is a char* under the hood, and 
    // you are reading things into the string using c_str to get the char*
    // you are constrained by all the requirements of dealing with a char*
    // Which means you have to preallocate the char* size 
    // so you have valid memory to write to
    // which in this case is done with the resize call
    model.resize(size_of_model);
    // read in the data for the model string
    infile.read((char*)model.c_str(), size_of_model);
    // read in the int for the number of wheels
    infile.read((char*)&new_car.num_of_wheels, sizeof(int));
    new_car.model = model.c_str();
    // loop through and read in all the features of the car
    for(int i = 0; i < feature_sizes.size(); ++i) {
        std::string feature;
        feature.resize(feature_sizes[i]);
        infile.read((char*)feature.c_str(), feature_sizes[i]);
        new_car.list_of_features.push_back(feature.c_str());
    }
    infile.close();
    return new_car;
}

int main(){
    Car car_one("suzuki", 4);
    car_one.list_of_features.push_back("cup holder");
    car_one.list_of_features.push_back("gps");
    write_to_binary_file(car_one);
    Car car_found_in_file = read_bin_of_cars("cars.bin");
    car_found_in_file.print_car_details();
    return 0;
}