/*
 * MuchObliged: decrypts a given SHRUG2 encrypted file
 */

#include <string>
#include <iostream>
#include <fstream>
#include <CryptoPP/aes.h>
#include <CryptoPP/ccm.h>
#include <CryptoPP/files.h>
#include <Windows.h>

std::string eraseSubstr(std::string str, std::string const& substr)
{
	size_t pos = str.find(substr);
	if (pos != std::string::npos)
		str.erase(pos, substr.size());
	return str;
}

std::string getRegKey(HKEY hKey, std::string const& subKey, std::string const& value)
{
	DWORD strSize = 0;
	// Acquire the size of the string before the dynamic allocation happens
	LSTATUS status = RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, nullptr, &strSize);
	if (status != ERROR_SUCCESS)
		throw std::exception("Couldn't read the given registry value");
	// Allocate a string with the size returned from RegGetValue
	std::string str;
	// Acquire size of the string in char
	str.resize(strSize / sizeof(char));
	RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, &str[0], &strSize);
	// Get rid of the last null terminator given by RegGetValue
	str.resize(strSize / sizeof(char) - 1);
	return str;
}

void decryptFile(std::ifstream &in, std::ofstream &out, std::string const& key, std::string const& iv)
{
	// Decrypt in AES-CBC mode	
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());
	CryptoPP::FileSource source(in, true,
		new CryptoPP::StreamTransformationFilter(decryption,
			new CryptoPP::FileSink(out)));
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cout << "[ * ] Please input an encrypted file!" << std::endl;
		std::cout << "[ * ] Also.. keep the .SHRUG2 extension." << std::endl;
		return 1;
	}
	std::cout << "[ * ] I am MuchObliged." << std::endl;
	std::cout << "[ ! ] WARNING: MuchObliged won't work if you're not running from an infected machine." << std::endl;
	// Read registry for key / IV
	std::string key, iv;
	try
	{
		key = getRegKey(HKEY_CURRENT_USER, "ShrugTwo", "crykey");
		iv = getRegKey(HKEY_CURRENT_USER, "ShrugTwo", "cryiv");
	}
	catch (std::exception &e)
	{
		std::cerr << "[ * ] Failed to read the registry for Key / IV (" << e.what() << ")" << std::endl;
		return 1;
	}
	std::cout << "[ * ] KEY: " << key << std::endl;
	std::cout << "[ * ] IV: " << iv << std::endl;
	// Read and decrypt our files: AES-CBC
	std::string encryptedName = argv[1];
	std::ifstream inFile(encryptedName, std::ios::binary);
	if (inFile.fail())
	{
		std::cerr << "[ * ] File '" << encryptedName << "' does not exist!" << std::endl;
		return 1;
	}
	// Is an encrypted file?
	if (encryptedName.substr(encryptedName.find_last_of(".") + 1) == "SHRUG2")
	{
		// Attempt to recover the original filename
		std::string decryptedName = eraseSubstr(encryptedName, ".SHRUG2");
		std::ofstream ofFile(decryptedName, std::ios::binary);
		decryptFile(inFile, ofFile, key, iv);
		std::cout << "[ * ] Wrote decrypted file to " << decryptedName << "!" << std::endl;
	}
	else
	{
		std::cerr << "[ * ] Is this really an encrypted file? Check the extension." << std::endl;
	}
	return 0;
}
