/*
 * KeyboardControlls_client.cpp
 *
 *  Created on: 2012/09/16
 *      Author: umakatsu
 */
#include "ARMM/KeyboardControlls_client.h"
#include "ARMM/Rendering/osg_Client.h"
#include "ARMM/Rendering/osg_Object.h"
#include "ARMM/Rendering/osg_geom_data.h"
#include "constant.h"

//OpenCV
#include <opencv/cv.h>

//Standard API
#include <iostream>
#include <cstring>

namespace ARMM
{

	using namespace std;

	extern int		collidedNodeInd;
	extern bool	collision;
	const float CUBE_SIZE = 4;
	const float SPHERE_SIZE = 2;//FULL SIZE

	extern std::string pDstModel; //actually this should be deal with member of Keyboard class, but some error occurred so could not do it...
	int KeyboardController_client::check_input()
	{
		if (getKey(78)) {//N
			return 78;
		} else if (getKey(83)) {//S
			return 83;
		} else if (getKey(65)) {//A
			return 65;
		} else if (getKey(68)) {//D
			return 68;
		}
		//A 65 S 83 D 68 W 87 F 70 V 86
		return 0;
	}

	void KeyboardController_client::set_input(const int & key, boost::shared_ptr<osg_Client> osgrender)
	{
		if(!osgrender)
		{
			cerr << "Error: No rendering object is detected!" << endl;
			return;
		}

		switch(key)
		{
			case 78://N
				osgrender->osgAddObjectNode(osgSphereNode(SPHERE_SIZE));
				break;

			case 65:
				break;

			case 66: //B
				osgrender->osgAddObjectNode(osgBoxNode(CUBE_SIZE));
				break;

			case 79: {//o
//				ostringstream str;
//				const char * FILENAME = "ColorTorus";
//				const char * FORMAT = ".3ds";
//				str << DATABASEDIR << FILENAME << "/" << FILENAME << FORMAT;
//				osg::ref_ptr<osg::Node> obj = osgDB::readNodeFile(str.str().c_str());
//				LoadCheck(obj.get(), str.str().c_str());
////				float scale = 0.005;//GreenTable
//				float scale = 10;//GreenTorus
//
				ostringstream str;
				const char * format = ".3ds";
				const char * filename = "ItimatsuCow";
//				if(mSrcModel.empty())
//				{
//					mSrcModel = filename;
//				}
//				else
//				{
//					m
//				}
				str << ConstParams::DATABASEDIR << filename << "/" << filename << format;
				osg::ref_ptr<osg::Node> obj = osgDB::readNodeFile(str.str().c_str());
				LoadCheck(obj.get(), str.str().c_str());
				string tmpFilename(filename);
				tmpFilename += format;
				obj->setName(tmpFilename);

				float scale = 10;

//				//安原モデルの場合、テクスチャ座標の調整が必要
//				string str(ConstParams::DATABASEDIR);
//				const char * FORMAT = ".3ds";
//				const char * FILENAME = "keyboard";
//				str += FILENAME; str+="/";
//				string modelFileName(FILENAME);
//				modelFileName += FORMAT;
//				osg::ref_ptr<osg::Node> obj = (osg3DSFileFromDiorama(modelFileName.c_str(), str.c_str()));
//				double scale = 300; //サーバ側の値から決定した for (keyboard) for 安原モデル
//				obj->setName(str);
				osgrender->osgAddObjectNode(obj.get(), scale);
				break;
			}

			case 80: {//p
				ostringstream str;
				const char * format = ".3ds";
				const char * filename = "cow";
				str << ConstParams::DATABASEDIR << filename << "/" << filename << format;
				osg::ref_ptr<osg::Node> obj = osgDB::readNodeFile(str.str().c_str());
				LoadCheck(obj.get(), str.str().c_str());
				string tmpFilename(filename);
				tmpFilename += format;
				obj->setName(tmpFilename);

				float scale = 10;

////				//安原モデルの場合、テクスチャ座標の調整が必要
//				string str(DATABASEDIR);
//				const char * FILENAME = "cube";
//				str += FILENAME; str+="/";
//				string modelFileName(FILENAME);
//				modelFileName += FORMAT;
//				osg::ref_ptr<osg::Node> obj = (osg3DSFileFromDiorama2(modelFileName.c_str(), str.c_str()));
//				obj->setName(str);
//				double scale = 20; //for (Cube) for 安原モデル ただしosg3DSFileFromDioramaで調整されていること
////				double scale = 300; //for (keyboard) for 安原モデル
				osgrender->osgAddObjectNode(obj.get(), scale);
				break;
			}


			case 68:
				break;
			case 83:
				break;

			case 100: {// This function is called after texture transferred
				if(pDstModel.empty())
				{
					cerr << "No name of dst model" << endl;
					return;
				}

				printf("Destination model = %s\n", pDstModel.c_str());

				ostringstream str;
//				const char * FILENAME = "Newcow";
//				str << ConstParams::DATABASEDIR << FILENAME << "/" << FILENAME << FORMAT;
				string dir = pDstModel.substr(0, pDstModel.size()-4);
				str << ConstParams::DATABASEDIR << "New" <<  dir.c_str() << "/" << "New" << pDstModel.c_str();

				cout << str.str().c_str() << endl;

				//remove the texture image object
				osg::ref_ptr<osg::Node> pObjTexture = osgrender->getOsgObject()->getObjTexture();
				osg::ref_ptr<osg::PositionAttitudeTransform> pObjTexturePosAtt = osgrender->getOsgObject()->getObjTexturePosAtt();
				osgrender->getShadowedScene()->removeChild(pObjTexture);
				pObjTexturePosAtt->removeChild(pObjTexture);
				osgrender->getOsgObject()->setSoftTexture(false);

				//getting the index of the child node
				vector<osg::Node*> pObjNodeArray
					= osgrender->getOsgObject()->getObjNodeArray();
				vector<osg::PositionAttitudeTransform *> pObjTransformArray
					= osgrender->getOsgObject()->getObjTransformArray();
				int childInd = pObjTransformArray[collidedNodeInd]
							   ->getChildIndex(pObjNodeArray[collidedNodeInd]);

				//swap a child of the objects node with new child node
				osg::ref_ptr<osg::Node> obj = osgDB::readNodeFile(str.str().c_str());
				LoadCheck(obj.get(), str.str().c_str());
				pObjTransformArray[collidedNodeInd]->setChild(childInd, obj.get());
				double scale = 10; //
//				double scale = 20; //安原Cube
//				double scale = 300;
				pObjTransformArray[collidedNodeInd]->setScale(osg::Vec3d(scale,scale,scale));
				collision = false;
				break;
			}

			//creating a texture sticked in hand when collision occurred
			case 101:
			{
				osg::ref_ptr<osg::Node> pObjTexture = osgrender->getOsgObject()->getObjTexture();

				//parts node
				string str(ConstParams::DATABASEDIR);
				const char * file = "LSCM_Newcube0.bmp";
				str += file;
				pObjTexture = osgCreateSoft(str.c_str());
				LoadCheck(pObjTexture.get(), str.c_str());

				osg::ref_ptr<osg::PositionAttitudeTransform> pObjTexturePosAtt = new osg::PositionAttitudeTransform();
				pObjTexturePosAtt->addChild(pObjTexture);
				pObjTexturePosAtt->getOrCreateStateSet()->setRenderBinDetails(2, "RenderBin");


				osgrender->getShadowedScene()->addChild(pObjTexture);
				osgrender->getOsgObject()->setObjTexture(pObjTexture);

//				osgrender->getShadowedScene()->addChild(pObjTexturePosAtt);
				osgrender->getOsgObject()->setObjTexturePosAtt(pObjTexturePosAtt);
//				osgrender->getOsgObject()->setSoftTexture(true);

				cout << "<<Create a texture image with ocurring collision>> " << endl;

				break;
			}

		}
		//A 65 S 83 D 68 W 87 F 70 V 86
	}

	bool KeyboardController_client::getKey(int key)
	{
		return false;
	}
}
