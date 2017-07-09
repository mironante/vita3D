#include "ObjLoader.hpp"
#include "../toolbox/math_utils.h"

#include <vector>

using namespace std;

namespace RenderEngine
{
    typedef struct {
        int vertex;
        int texture;
        int normal;
    } VertexInfo;

    RawModel* ObjLoader::loadObjModel(std::string fileName, Loader* loader)
    {
		FILE *fp = fopen(fileName.c_str(),"rb");

        if (!fp)
        {
            return 0;
        }

        char ch				= 0;
        char ch2			= 0;
        char strLine[255]	= {0};
        char buff[512];

        vector<vector3f> vertices;
        vector<vector2f> textures;
        vector<vector3f> normals;

        vector<VertexInfo> vertexInfos;

        Vertex* outVertices;

		while(!feof(fp))
		{
			//get first character
			ch = fgetc(fp);

			switch(ch)
			{
				case 'v': //vertex found
				{
					//get second character
					ch2 = fgetc(fp);

					switch(ch2)
					{
						case ' ': // vertex
						{
                            vector3f vertex;
                            fscanf(fp,"%f %f %f", &vertex.x, &vertex.y, &vertex.z);
                            vertices.push_back(vertex);
                            fgets(strLine, 100, fp);
						}
						break;

						case 't': // texture
						{
                            vector2f texture;
                            fscanf(fp,"%f %f", &texture.x, &texture.y);
                            textures.push_back(texture);
                            fgets(strLine, 100, fp);
                        }
						break;

						case 'n': // normal
                        {
                            vector3f normal;
                            fscanf(fp,"%f %f %f", &normal.x, &normal.y, &normal.z);
                            normals.push_back(normal);
                            fgets(strLine, 100, fp);
                        }
                        break;

						default:
							break;
						}
					}
					break;

				case 'f': //face info
                {
                    fgets (buff, sizeof (buff), fp);
                    VertexInfo vi1, vi2, vi3;
                    sscanf(buff,"%d/%d/%d %d/%d/%d %d/%d/%d",
                    &vi1.vertex, &vi1.texture, &vi1.normal,
                    &vi2.vertex, &vi2.texture, &vi2.normal, 
                    &vi3.vertex, &vi3.texture, &vi3.normal);
                    vertexInfos.push_back(vi1);
                    vertexInfos.push_back(vi2);
                    vertexInfos.push_back(vi3);
                }
                break;

				case '\n':
				    break;

				default:
					//read line to the end
					fgets(strLine, 100, fp);
					break;
				}
			}

			fclose(fp);

            int vertsCount = vertices.size();
            Vertex* verts = new Vertex[vertsCount];

            int indicesCount = vertexInfos.size();
            uint16_t* indices = new uint16_t[indicesCount];

            for(std::vector<int>::size_type i = 0; i != vertexInfos.size(); i++) {
                int vertexPointer = vertexInfos[i].vertex - 1;
                int texturePointer = vertexInfos[i].texture - 1;
                int normalPointer = vertexInfos[i].normal - 1;

                indices[i] = vertexPointer;

                verts[vertexPointer].x = vertices[vertexPointer].x;
                verts[vertexPointer].y = vertices[vertexPointer].y;
                verts[vertexPointer].z = vertices[vertexPointer].z;

                verts[vertexPointer].u = textures[texturePointer].x;
                verts[vertexPointer].v = 1.0f - textures[texturePointer].y;

                verts[vertexPointer].nx = normals[normalPointer].x;
                verts[vertexPointer].ny = normals[normalPointer].y;
                verts[vertexPointer].nz = normals[normalPointer].z;
            }

            RawModel* model = loader->loadModel(verts, vertsCount, indices, indicesCount);
			return model;
		}
}