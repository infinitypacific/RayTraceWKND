// frostimg.cpp : Defines the entry point for the application.
//

#include "frostimg.h"
#include "datatool.h"
#include <iostream>
#include <stdint.h>

frostbmpimg::frostbmpimg(uint_fast32_t width = 0, uint_fast32_t height = 0, char dr = 0, char dg = 0, char db = 0)
{
	header.type[0] = 'B';
	header.type[1] = 'M';
	infoheader.rowpad = (4 * ceil(float(width * 3) / 4.0)) - width * 3;
	header.offset = 54;
	header.reserved1 = 0;
	header.reserved2 = 0;
	header.size = 54+height*(width*3+infoheader.rowpad);
	infoheader.compression = 0;
	infoheader.size = 40;
	infoheader.width = width;
	infoheader.height = height;
	raster = new char[width * height][3];
	infoheader.planes = 1;
	infoheader.bitc = 24;
	infoheader.xperm = uint_fast32_t(2834);
	infoheader.yperm = uint_fast32_t(2834);
	corrupt = false;
	for (uint_fast32_t i = 0;i < width * height;i++) { raster[i][0] = db;raster[i][1] = dg;raster[i][2] = dr; }
}

frostbmpimg::frostbmpimg(std::string name)
{
	std::ifstream rfile(name, std::ios::binary); // + ".bmp"
	if (!rfile.is_open()) { corrupt = true;return; } //1
	header.size = rfile.tellg();
	rfile.seekg(0, std::ios::end);
	header.size = static_cast<uint_fast32_t>(rfile.tellg()) - header.size;
	rfile.seekg(0);
	char tempbuf2[2];
	rfile.read(tempbuf2, 2);
	if (tempbuf2[0] != 'B' && tempbuf2[1] != 'M') { corrupt = true;return; } //2
	header.type[0] = tempbuf2[0];
	header.type[1] = tempbuf2[1];
	char tempbuf4[4];
	rfile.read(tempbuf4, 4);
	uint_fast32_t filesized = dtt::byte2ufast32(tempbuf4, true);
	if (filesized != header.size) { std::cout << "Warning: BMP File has file size mismatch." << std::endl; }
	rfile.read(tempbuf2, 2); //reserved
	header.reserved1 = dtt::byte2ufast16(tempbuf2, true);
	rfile.read(tempbuf2, 2); //reserved
	header.reserved2 = dtt::byte2ufast16(tempbuf2, true);
	rfile.read(tempbuf4, 4);
	header.offset = dtt::byte2ufast32(tempbuf4, true); //uint_fast32_t rasteroff
	rfile.read(tempbuf4, 4);
	infoheader.size = dtt::byte2ufast32(tempbuf4, true); //uint_fast32_t infosize
	if (infoheader.size != 40) { corrupt = true;return; }
	rfile.read(tempbuf4, 4);
	infoheader.width = dtt::byte2ufast32(tempbuf4, true); //uint_fast32_t width
	rfile.read(tempbuf4, 4);
	infoheader.height = dtt::byte2ufast32(tempbuf4, true); //uint_fast32_t height
	rfile.read(tempbuf2, 2);
	infoheader.planes = dtt::byte2ufast16(tempbuf2, true);
	if (infoheader.planes != 1) { corrupt = true;return; }
	rfile.read(tempbuf2, 2);
	infoheader.bitc = dtt::byte2ufast16(tempbuf2, true);
	switch (infoheader.bitc) {
	case 16:
	case 24:
		break;
	default:
		corrupt = true;return;
	}
	rfile.read(tempbuf4, 4);
	infoheader.compression = dtt::byte2ufast32(tempbuf4, true);
	if (infoheader.compression != 0) { corrupt = true;return; }
	rfile.read(tempbuf4, 4); //Size (doesn't matter)
	rfile.read(tempbuf4, 4);
	infoheader.xperm = dtt::byte2ufast32(tempbuf4, true);
	rfile.read(tempbuf4, 4);
	infoheader.yperm = dtt::byte2ufast32(tempbuf4, true);
	rfile.read(tempbuf4, 4); //ColorsUsed
	infoheader.colorsused = dtt::byte2ufast32(tempbuf4, true);
	rfile.read(tempbuf4, 4); //ColorsImportant
	infoheader.colorsimportant = dtt::byte2ufast32(tempbuf4, true);
	const uint_fast8_t byteperpix = (infoheader.bitc == 24 ? 3 : 2);
	infoheader.rowpad = (4 * ceil(float(infoheader.width * byteperpix) / 4.0)) - infoheader.width * byteperpix;
	rfile.seekg(header.offset);
	raster = new char[infoheader.width * infoheader.height][3];
	char tempbuf3[3];
	char tempbuf;
	switch (infoheader.bitc) {
	case 24:
		for (uint_fast32_t y = 0;y < infoheader.height;y++) {
			for (uint_fast32_t x = 0;x < infoheader.width;x++) {
				uint_fast32_t pos = (y * infoheader.width) + x;
				rfile.read(tempbuf3, 3);
				raster[pos][0] = tempbuf3[0];
				raster[pos][1] = tempbuf3[1];
				raster[pos][2] = tempbuf3[2];
			}
			for (uint_fast8_t i = 0;i < infoheader.rowpad;i++) { rfile.read(&tempbuf, 1); }
		}
		break;
	case 16:
		for (int_fast32_t y = infoheader.height - 1;y >= 0;y--) {
			for (uint_fast32_t x = 0;x < infoheader.width;x++) {
				uint_fast32_t pos = (y * infoheader.width) + x;
				rfile.read(tempbuf2, 2);
				raster[pos][0] = tempbuf2[0];
				raster[pos][1] = tempbuf2[1];
			}
			for (uint_fast8_t i = 0;i < infoheader.rowpad;i++) { rfile.read(&tempbuf, 1); }
		}
		break;
	}
	rfile.close();
}

void frostbmpimg::writeFile(std::string name) const
{
	std::ofstream wfile(name, std::ios::binary); // + ".bmp"
	wfile.write(header.type, 2);
	wfile.write(dtt::ufast322byte(header.size,true).data(), 4);
	wfile.write(dtt::ufast162byte(header.reserved1, true).data(), 2);
	wfile.write(dtt::ufast162byte(header.reserved2, true).data(), 2);
	wfile.write(dtt::ufast322byte(header.offset, true).data(), 4);
	wfile.write(dtt::ufast322byte(infoheader.size, true).data(), 4);
	wfile.write(dtt::ufast322byte(infoheader.width, true).data(), 4);
	wfile.write(dtt::ufast322byte(infoheader.height, true).data(), 4);
	wfile.write(dtt::ufast162byte(infoheader.planes, true).data(), 2);
	wfile.write(dtt::ufast162byte(infoheader.bitc, true).data(), 2);
	wfile.write(dtt::ufast322byte(infoheader.compression, true).data(), 4);
	//Add compression in future update :)
	wfile.write(dtt::ufast322byte(uint_fast32_t(0), true).data(), 4); //ImageSize
	wfile.write(dtt::ufast322byte(infoheader.xperm, true).data(), 4); //XPPM
	wfile.write(dtt::ufast322byte(infoheader.yperm, true).data(), 4); //YPPM
	wfile.write(dtt::ufast322byte(uint_fast32_t(0), true).data(), 4); //Colors Used
	wfile.write(dtt::ufast322byte(uint_fast32_t(0), true).data(), 4); //Important
	char czro = 0;
	for (uint_fast32_t y = 0; y < infoheader.height;y++) {
		for (uint_fast32_t x = 0;x < infoheader.width;x++) {
			uint_fast32_t post = (y * infoheader.width) + x;
			switch (infoheader.bitc) {
			case 24:
				wfile.write(&raster[post][0], 3);
				break;
			case 16:
				wfile.write(&raster[post][0], 2);
				std::cout << "16" << std::endl;
				break;
			default:
				std::cout << "Invalid bitcount: " << infoheader.bitc << std::endl;
			}
		}
		for (uint_fast8_t i = 0;i < infoheader.rowpad;i++) { wfile.write(&czro, 1);std::cout << "spaced" << std::endl; }
	}
	wfile.close();
}

frost::databuf frostbmpimg::writeBuf() const
{
	frost::databuf buffer;
	buffer.data = new unsigned char[header.size];
	buffer.size = header.size;
	uint_fast32_t cursor = 54;
	std::array<char, 4> tempbuf4;
	std::array<char, 3> tempbuf3;
	std::array<char, 2> tempbuf2;
	buffer.data[0] = header.type[0];
	buffer.data[1] = header.type[1];
	tempbuf4 = dtt::ufast322byte(header.size,true);
	buffer.data[2] = tempbuf4[0];
	buffer.data[3] = tempbuf4[1];
	buffer.data[4] = tempbuf4[2];
	buffer.data[5] = tempbuf4[3];
	tempbuf2 = dtt::ufast162byte(header.reserved1, true);
	buffer.data[6] = tempbuf2[0];
	buffer.data[7] = tempbuf2[1];
	tempbuf2 = dtt::ufast162byte(header.reserved2, true);
	buffer.data[8] = tempbuf2[0];
	buffer.data[9] = tempbuf2[1];
	tempbuf4 = dtt::ufast322byte(header.offset, true);
	buffer.data[10] = tempbuf4[0];
	buffer.data[11] = tempbuf4[1];
	buffer.data[12] = tempbuf4[2];
	buffer.data[13] = tempbuf4[3];
	tempbuf4 = dtt::ufast322byte(infoheader.size, true);
	buffer.data[14] = tempbuf4[0];
	buffer.data[15] = tempbuf4[1];
	buffer.data[16] = tempbuf4[2];
	buffer.data[17] = tempbuf4[3];
	tempbuf4 = dtt::ufast322byte(infoheader.width, true);
	buffer.data[18] = tempbuf4[0];
	buffer.data[19] = tempbuf4[1];
	buffer.data[20] = tempbuf4[2];
	buffer.data[21] = tempbuf4[3];
	tempbuf4 = dtt::ufast322byte(infoheader.height, true);
	buffer.data[22] = tempbuf4[0];
	buffer.data[23] = tempbuf4[1];
	buffer.data[24] = tempbuf4[2];
	buffer.data[25] = tempbuf4[3];
	tempbuf2 = dtt::ufast162byte(infoheader.planes, true);
	buffer.data[26] = tempbuf2[0];
	buffer.data[27] = tempbuf2[1];
	tempbuf2 = dtt::ufast162byte(infoheader.bitc, true);
	buffer.data[28] = tempbuf2[0];
	buffer.data[29] = tempbuf2[1];
	tempbuf4 = dtt::ufast322byte(infoheader.compression, true);
	buffer.data[30] = tempbuf4[0];
	buffer.data[31] = tempbuf4[1];
	buffer.data[32] = tempbuf4[2];
	buffer.data[33] = tempbuf4[3];
	buffer.data[34] = 0;
	buffer.data[35] = 0;
	buffer.data[36] = 0;
	buffer.data[37] = 0;
	tempbuf4 = dtt::ufast322byte(infoheader.xperm, true);
	buffer.data[38] = tempbuf4[0];
	buffer.data[39] = tempbuf4[1];
	buffer.data[40] = tempbuf4[2];
	buffer.data[41] = tempbuf4[3];
	tempbuf4 = dtt::ufast322byte(infoheader.yperm, true);
	buffer.data[42] = tempbuf4[0];
	buffer.data[43] = tempbuf4[1];
	buffer.data[44] = tempbuf4[2];
	buffer.data[45] = tempbuf4[3];
	buffer.data[46] = 0;
	buffer.data[47] = 0;
	buffer.data[48] = 0;
	buffer.data[49] = 0;
	buffer.data[50] = 0;
	buffer.data[51] = 0;
	buffer.data[52] = 0;
	buffer.data[53] = 0;
	for (uint_fast32_t y = 0; y < infoheader.height;y++) {
		for (uint_fast32_t x = 0;x < infoheader.width;x++) {
			uint_fast32_t post = (y * infoheader.width) + x;
			switch (infoheader.bitc) {
			case 24:
				buffer.data[cursor++] = raster[post][0];
				buffer.data[cursor++] = raster[post][1];
				buffer.data[cursor++] = raster[post][2];
				break;
			case 16:
				buffer.data[cursor++] = raster[post][0];
				buffer.data[cursor++] = raster[post][1];
				break;
			default:
				std::cout << "Invalid bitcount: " << infoheader.bitc << std::endl;
			}
		}
		for (uint_fast8_t i = 0;i < infoheader.rowpad;i++) { buffer.data[cursor++] = 0; }
	}
	return buffer;
}

frostbmpimg::~frostbmpimg() {
	delete[] raster;
}

std::array<char, 4> frostbmpimg::getPixel(uint_fast32_t x, uint_fast32_t y) const
{
	uint_fast32_t pos = (((infoheader.height-y-1) * infoheader.width) + x);
	switch (infoheader.bitc) {
	case 24:
		return std::array<char, 4>({ raster[pos][2], raster[pos][1], raster[pos][0], 0});
		break;
	case 16:
		return std::array<char, 4>({raster[pos][1] >> 2, (raster[pos][0] >> 5) | ((raster[pos][1] << 3) & 0x03), raster[pos][0] & 0x1f, 0});
		break;
	}
	return std::array<char, 4>();
}

bool frostbmpimg::setPixel(uint_fast32_t x, uint_fast32_t y, char r, char g, char b)
{
	uint_fast32_t pos = (((infoheader.height - y - 1) * infoheader.width) + x);
	switch (infoheader.bitc) {
	case 24:
		raster[pos][0] = b;
		raster[pos][1] = g;
		raster[pos][2] = r;
		break;
	case 16:
		raster[pos][0] = (b & 0x1f) | (g << 5);
		raster[pos][1] = ((g >> 3) & 0x03) | ((r << 2) & 0x7c);
		break;
	default:
		return false;
	}
	return true;
}

void frostbmpimg::setSize(uint_fast32_t width, uint_fast32_t height)
{
	infoheader.width = width;
	infoheader.height = height;
	delete[] raster;
	raster = new char[width * height][3];
	infoheader.rowpad = (4 * ceil(float(width * 3) / 4.0)) - width * 3;
	header.size = 54 + height * (width * 3 + infoheader.rowpad);
}

frimg::bmpHeader frostbmpimg::getBmpHeader() const
{
	return header;
}

void frostbmpimg::setBmpHeader(frimg::bmpHeader iheader)
{
	header = iheader;
}

frimg::infoHeader frostbmpimg::getInfoHeader() const
{
	return infoheader;
}

void frostbmpimg::setInfoHeader(frimg::infoHeader iheader)
{
	infoheader = iheader;
}

uint_fast32_t frostbmpimg::getWidth() const
{
	return infoheader.width;
}

uint_fast32_t frostbmpimg::getHeight() const
{
	return infoheader.height;
}

void frostbmpimg::setRaster(char(*nraster)[3])
{
	raster = nraster;
}

void frostbmpimg::setBit(uint_fast16_t nbitc)
{
	infoheader.bitc = nbitc;
	const uint_fast8_t byteperpix = (nbitc == 24 ? 3 : 2);
	infoheader.rowpad = (4 * ceil(float(infoheader.width * byteperpix) / 4.0)) - infoheader.width * byteperpix;
	header.size = 54 + infoheader.height * (infoheader.width * byteperpix + infoheader.rowpad);
}

bool frostbmpimg::isCorrupt() const
{
	return corrupt;
}

std::array<char, 4> frostbmpimg::operator[](int index) const
{
	return std::array<char, 4>({ raster[index][2], raster[index][1],  raster[index][0], 0 });
}
