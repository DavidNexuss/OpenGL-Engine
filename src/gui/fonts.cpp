#include <directory.h>
#include "fonts.h"
using namespace std;

namespace GUI
{

	size_t loadFont(const string& fontname,int size) 
	{
		return loadFont(fontname,size,fonts.size());
	}

    size_t loadFont(const string& fontname,int size,size_t id)
	{
		string path = (string(Directory::fontPaths) + fontname);
		ImFont* pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size);
		if (id == fonts.size()) fonts.push_back(pFont);
		else fonts[id] = pFont;

		return id;
	}
    
	void setFont(int id)
	{
		ImGui::PushFont(fonts[id]);
	}
}