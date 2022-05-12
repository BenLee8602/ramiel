#include <algorithm>
#include <sstream>
#include "objloader.h"

namespace bl::objloader {

    std::vector<size_t> splitv(const std::string& in) {
        std::stringstream stream(in);
        std::vector<size_t> tokens;
        std::string token;
        while (getline(stream, token, '/')) {
            if (token.length()) tokens.push_back(stoi(token));
            else tokens.push_back(0);
        }
        return tokens;
    }


    void count(const char* filename, size_t& v, size_t& vt, size_t& f) {
        v = 0;
        vt = 0;
        f = 0;

        std::fstream file(filename);
        if (!file.is_open()) throw std::string("bl::objloader::count - file " + std::string(filename) + " not found");

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream stream(line);

            std::string type;
            stream >> type;

            if (type == "v") ++v;
            else if (type == "vt") ++vt;
            else if (type == "f") {
                f += std::count(line.begin(), line.end(), ' ') - 2;
            }
        }

        file.close();
    }


    void load(const char* filename, std::vector<Vec3f>& v, std::vector<Vec2f>& v_txt, std::vector<Vec3u>& tri, std::vector<Vec3u>& tri_txt) {
        std::fstream file(filename);
        if (!file.is_open()) throw std::string("bl::objloader::load - file " + std::string(filename) + " not found");

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream stream(line);
            
            std::string type;
            stream >> type;

            if (type == "v") {
				Vec3f temp;
				stream >> temp[X] >> temp[Y] >> temp[Z];
				v.push_back(temp);
			}

			else if (type == "vt") {
				Vec2f temp;
				stream >> temp[U] >> temp[V];
				v_txt.push_back(temp);
			}

            else if (type == "f") {
                bool isQuad = std::count(line.begin(), line.end(), ' ') == 4;
                
                Vec3u pos = { 0, 0, 0 };
                Vec3u txt = { 0, 0, 0 };
                for (int i = 0; i < 3; ++i) {
                    std::string v;
                    stream >> v;
                    std::vector<size_t> comp = splitv(v);
                    pos[i] = comp[0] - 1;
                    if (comp.size() > 1 && comp[1]) txt[i] = comp[1] - 1;
                }
                tri.push_back(pos);
                if (txt[0] || txt[1] || txt[2]) tri_txt.push_back(txt);

                if (isQuad) {
                    unsigned pos4;
                    unsigned txt4;
                    std::string v;
                    stream >> v;
                    std::vector<size_t> comp = splitv(v);
                    pos4 = comp[0] - 1;
                    if (comp.size() > 1 && comp[1]) {
                        txt4 = comp[1] - 1;
                    }
                    tri.push_back(Vec3u{ pos[2], pos4, pos[0] });
                    if (txt) tri_txt.push_back({ txt[2], txt4, txt[0] });
                }
            }
        }
    }

}
