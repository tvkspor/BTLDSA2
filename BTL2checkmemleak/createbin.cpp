#include <iostream>
#include <fstream>

int main() {
    // Tạo một mảng chứa 10 số nguyên
    int numbers[] = {10, 890, 42, 721, 956, 318, 647, 29, 184, 537};

    // Mở file để ghi dữ liệu dưới dạng nhị phân
    std::ofstream outFile("INT10.BIN", std::ios::binary);

    // Kiểm tra xem file có mở thành công không
    if (!outFile.is_open()) {
        std::cerr << "khong the mo file INT10.BIN de ghi." << std::endl;
        return 1;
    }

    // Ghi dữ liệu vào file
    outFile.write(reinterpret_cast<const char*>(numbers), sizeof(numbers));

    // Đóng file
    outFile.close();

    std::cout << "File INT10.BIN da duoc tao thanh cong." << std::endl;

    return 0;
}
