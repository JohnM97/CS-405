// Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include <filesystem>  // For file size check
#include <ctime>

/// <summary>
/// encrypt or decrypt a source string using the provided key
/// </summary>
/// <param name="source">input string to process</param>
/// <param name="key">key to use in encryption / decryption</param>
/// <returns>transformed string</returns>
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
    // Check that the source and key are not empty before proceeding
    if (source.empty()) {
        std::cerr << "Error: Source string is empty." << std::endl;
        return {};
    }

    if (key.empty()) {
        std::cerr << "Error: Key cannot be empty." << std::endl;
        return {};
    }

    const size_t source_length = source.length();
    const size_t key_length = key.length();
    std::string output;
    output.reserve(source_length);  // Reserve memory to improve performance

    // Encrypt or decrypt each character using XOR with the key, cycling through key if needed
    for (size_t i = 0; i < source_length; ++i) {
        char encrypted_char = source[i] ^ key[i % key_length];
        output.push_back(encrypted_char);
    }

    // Return the transformed string
    return output;
}


std::string read_file(const std::string& filename)
{
    std::ifstream input_file(filename);
    std::stringstream buffer;

    // Check if the file opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open input file: " << filename << std::endl;
        return {};
    }

   
    if (std::filesystem::file_size(filename) == 0) {
        std::cerr << "Warning: File is empty: " << filename << std::endl;
        return {};
    }

    // Read the entire contents of the file into the buffer
    buffer << input_file.rdbuf();

    // Close the file handle explicitly
    input_file.close();

    // Return the full contents as a single string
    return buffer.str();
}

std::string get_student_name(const std::string& string_data)
{
    std::string student_name;

    // find the first newline
    size_t pos = string_data.find('\n');
    // did we find a newline
    if (pos != std::string::npos)
    { // we did, so copy that substring as the student name
        student_name = string_data.substr(0, pos);
    }

    return student_name;
}

void save_data_file(const std::string& filename, const std::string& student_name, const std::string& key, const std::string& data)
{
    // Validate inputs before attempting to write
    if (filename.empty() || student_name.empty() || key.empty() || data.empty()) {
        std::cerr << "Error: Missing input for file save." << std::endl;
        return;
    }

    std::ofstream output_file(filename, std::ios::out);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open output file: " << filename << std::endl;
        return;
    }

    // Get the current date formatted as yyyy-mm-dd
    time_t now = time(nullptr);
    tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &now);  // Thread-safe conversion for Windows
#else
    local_tm = *localtime(&now);   // Standard POSIX version
#endif
    char date_buffer[11];
    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &local_tm);

    // Write formatted output to file
    // Line 1: student name
    // Line 2: current date
    // Line 3: key
    // Line 4+: data
    output_file << student_name << '\n';
    output_file << date_buffer << '\n';
    output_file << key << '\n';
    output_file << data;

    output_file.close();  // Explicitly close file to release handle
}

int main()
{
    std::cout << "Encyption Decryption Test!" << std::endl;

    // input file format
    // Line 1: <students name>
    // Line 2: <Lorem Ipsum Generator website used> https://pirateipsum.me/ (could be https://www.lipsum.com/ or one of https://www.shopify.com/partners/blog/79940998-15-funny-lorem-ipsum-generators-to-shake-up-your-design-mockups)
    // Lines 3+: <lorem ipsum generated with 3 paragraphs> 
    //  Fire in the hole bowsprit Jack Tar gally holystone sloop grog heave to grapple Sea Legs. Gally hearties case shot crimp spirits pillage galleon chase guns skysail yo-ho-ho. Jury mast coxswain measured fer yer chains man-of-war Privateer yardarm aft handsomely Jolly Roger mutiny.
    //  Hulk coffer doubloon Shiver me timbers long clothes skysail Nelsons folly reef sails Jack Tar Davy Jones' Locker. Splice the main brace ye fathom me bilge water walk the plank bowsprit gun Blimey wench. Parrel Gold Road clap of thunder Shiver me timbers hempen halter yardarm grapple wench bilged on her anchor American Main.
    //  Brigantine coxswain interloper jolly boat heave down cutlass crow's nest wherry dance the hempen jig spirits. Interloper Sea Legs plunder shrouds knave sloop run a shot across the bow Jack Ketch mutiny barkadeer. Heave to gun matey Arr draft jolly boat marooned Cat o'nine tails topsail Blimey.

    const std::string file_name = "inputdatafile.txt";
    const std::string encrypted_file_name = "encrypteddatafile.txt";
    const std::string decrypted_file_name = "decrypteddatafile.txt"; 
    const std::string source_string = read_file(file_name);
    const std::string key = "password";

    // get the student name from the data file
    const std::string student_name = get_student_name(source_string);

    // encrypt sourceString with key
    const std::string encrypted_string = encrypt_decrypt(source_string, key);

    // save encrypted_string to file
    save_data_file(encrypted_file_name, student_name, key, encrypted_string);

    // decrypt encryptedString with key
    const std::string decrypted_string = encrypt_decrypt(encrypted_string, key);

    // save decrypted_string to file
    save_data_file(decrypted_file_name, student_name, key, decrypted_string);

    if (decrypted_string != source_string) {
        std::cerr << "Warning: Decryption failed. The decrypted text does not match the original." << std::endl;
    }
    else {
        std::cout << "Success: Decrypted data matches the original input." << std::endl;
    }

    std::cout << "Read File: " << file_name << " - Encrypted To: " << encrypted_file_name << " - Decrypted To: " << decrypted_file_name << std::endl;

    // students submit input file, encrypted file, decrypted file, source code file, and key used
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
