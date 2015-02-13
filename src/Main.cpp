#include <iostream>
#include <fstream>

#include "Tracer.h"

using namespace std;


void
ParseSceneConfig(const char *sceneConfigFileName, Scene *scene)
{
    ifstream fCfg(sceneConfigFileName);
    if (!fCfg) {
        cerr << "Invalid scene config path!";
        exit(1);
    }
    
    char fname[256];
    float cen_x, cen_y, cen_z, rotation, inclination, scale_x, scale_y, scale_z;
    while (fCfg >> fname >> cen_x >> cen_y >> cen_z >> rotation >> inclination
               >> scale_x >> scale_y >> scale_z)
    {
        //cout << fname << endl << cen_x << " " << cen_y << " " << cen_z << endl;
        //cout << rotation << " " << inclination << endl;
        //cout << scale_x << " " << scale_y << " " << scale_z << endl;
        scene->AddObject(
            fname,                               //name of file with model
            glm::vec3(cen_x, cen_y, cen_z),      //center of model in world coords
            rotation,                            //rotation angle of model
            inclination,                         //inclination angle of model
            glm::vec3(scale_x, scale_y, scale_z) //scale
        );
    }
    
    if (!fCfg.eof()) {
        cerr << "Invalid scene config!";
        exit(1);
    }
    
    fCfg.close();
}

void ParseLightConfig(const char *lightConfigFileName, Scene *scene)
{
    ifstream fCfg(lightConfigFileName);
    if (!fCfg) {
        cerr << "Invalid light config path!";
        exit(1);
    }
    
    float red, green, blue, ambient, diffuse, specular;
    while (fCfg >> red >> green >> blue >> ambient >> diffuse >> specular) {
        scene->AddLightSource(
            Light(glm::vec3(red, green, blue), ambient, diffuse, specular)
        );
    }
    
    if (!fCfg.eof()) {
        cerr << "Invalid light config!";
        exit(1);
    }
    
    fCfg.close();
}

int
main(int argc, char *argv[])
{
    int xRes = 512;  // Default resolution
    int yRes = 512;
    
    if (argc == 4) { 
        ifstream fCfg(argv[1]);
        
        if (fCfg) {
            int xResFromFile = 0;
            int yResFromFile = 0;
            if (fCfg >> xResFromFile >> yResFromFile) {
                xRes = xResFromFile;
                yRes = yResFromFile;
            }
            else {
                cout << "Invalid resolution config format! Using default parameters." << endl;
            }
            fCfg.close();
        } else {
            cout << "Invalid resolution config path! Using default parameters." << endl;
        }
    }
    else {
        cerr << "Usage: ./main %ResolutionConfig% %SceneConfig% %LightConfig" << endl;
        return 1;
    }
    
    Camera camera;
    Scene scene;
    ParseSceneConfig(argv[2], &scene);
    ParseLightConfig(argv[3], &scene);
    Tracer tracer(&scene, camera);
    tracer.RenderImage(xRes, yRes, M_PI / 3.0, M_PI / 3.0);
    tracer.SaveImageToFile("../../Result.png");
}
