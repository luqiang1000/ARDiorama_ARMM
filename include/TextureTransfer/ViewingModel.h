// to use EIGEN sparse solver
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET

#ifndef VIEWINGMODEL_H_
#define VIEWINGMODEL_H_

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "OpenGL.h"
#include "TextureTransfer/BasicFace.h"

#include <Eigen/Sparse>
#include <Eigen/Core>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <deque>
#include <cstring>

//-------------------------------------------------------------------
// Structs
//-------------------------------------------------------------------
class Lib3dsFile;
namespace TextureTransfer
{
	class LSCM;
	class Texture;

	//メッシュ構造体
	struct Mesh
	{
	  unsigned int nIndex;
	  std::vector<float> vertex;
	  std::vector<float> normal;
	  std::vector<std::pair<int,Vector2> > mTextureCoords; // first: the index of selected vertex, second: the texture coords of selected vertex
	  std::vector<unsigned int> index;
	  std::vector<unsigned int> ind;

	  std::vector<Vector3> ambient, diffuse, specular;
	  std::vector<int> materials;

	  int ind_max;
	  int flag;
	  void reset(){
		  vertex.clear();
		  normal.clear();
		  mTextureCoords.clear();
		  index.clear();
		  ind.clear();
		  nIndex = 0;
		  ind_max = 0;
		  flag = 0;
	  }
	};

	//-------------------------------------------------------------------
	// Class definition
	//-------------------------------------------------------------------
	class ViewingModel{
	 public:
	  ViewingModel(const char * name = NULL);
	  ~ViewingModel();

	  bool RunLSCM();
	  void Save3DModel(const char * filename);

	  bool LoadTexture(const char * filename);
	  void ConvertDataStructure();
	  bool CheckFittingVertices(GLint *viewport, GLdouble *modelview, GLdouble *projection, cv::Point2d start_point, cv::Point2d end_point);
	  void UpdateMatrix();
	  void CorrespondTexCoord(GLint *viewport, GLdouble *modelview, GLdouble *projection,
			  cv::Point2d start_point, cv::Point2d end_point, Vector2 & t1, Vector2 & t2, Vector3 & p1, Vector3 & p2);
	  void RenewMeshDataConstruct(const int & separateNumber);

	  void  QueryNormal(const int & outer_loop, const int & mesh_index, GLdouble * normal);
	  void  QueryVertex(const int & outer_loop, const int & mesh_index, GLdouble * vertex);
	  void  QueryAmbient(const int & outer_loop, const int & mesh_index, GLfloat * ambient);
	  void  QueryDiffuse(const int & outer_loop, const int & mesh_index, GLfloat * diffuse);
	  void  QuerySpecular(const int & outer_loop, const int & mesh_index, GLfloat  * specular);
	  double QueryVertexColor(const int & outer_loop, const int & mesh_index) const;

	  int GetMeshSize() const;
	  int GetMeshIndicesSum(const int & outer_loop) const;
	  int GetMeshFlag(const int & outer_loop) const;
	  void IncrementSumOfStrokes();

		bool IsMeshSelected() const {
			return mMeshSelected;
		}

		void SetMeshSelected(bool meshSelected) {
			mMeshSelected = meshSelected;
		}

	 private:
	  void Load3DModel();
	  void VertexCorrection();
	  bool LoadMatrix();

	  void Load3DSModel();
	  void LoadObjModel();
	  std::deque<Texture *> LoadTextures(::Lib3dsFile * pModel, std::string dirpath);

	  void SetSelectedMeshData(const int& loopVer);

	 public:
	//  std::vector<boost::shared_ptr<LSCM>> mLSCM;
	  boost::shared_ptr<LSCM> mLSCM;

	  std::vector<Texture*> mTexture;

	  float mScales;
	  double mAngles[3];
	  double mTrans[3];
	//  std::deque<Texture *> texturesList; // A set of Textures 2011.6.7
	  std::pair<int,Mesh> mSelectedMesh; //first : an index for the matrix having harmonic field(mTexparts), second: a set of meshes

	 private:

	  char * mModelname;
	  int mSumOfVertices; 									// the number of vertices of this model
	  int mSumOfIndices; 									// the number of strokes added to this model
	  std::vector<int> mMinStartIndex;
	  std::vector<int> mMinEndIndex;

	  std::vector<Mesh> mMesh;

	  Eigen::SparseMatrix<double> sparse_laplacian;
	  Eigen::VectorXd b,mHarmonicValue;

	  bool mIsConvert;
	  bool mIsLoadMatrix;
	  bool mHasTexture;
	  bool mMeshSelected;
	};
}
#endif
