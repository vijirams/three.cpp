//
// Created by byter on 30.09.17.
//

#ifndef THREE_QT_SHADERMATERIAL_H
#define THREE_QT_SHADERMATERIAL_H

#include "Material.h"
#include <helper/Shader.h>

namespace three {

class ShaderMaterial : public Material
{
public:
  //defines = {};
  UniformValues uniforms;

private:
  const char *vertexShader = "void main() {\n\tgl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );\n}";
  const char * fragmentShader = "void main() {\n\tgl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );\n}";

  bool clipping = false; // set to use user-defined clipping planes
  bool morphNormals = false; // set to use morph normals

  /*
  bool ext_derivatives = false; // set to use derivatives
  bool ext_fragDepth = false; // set to use fragment depth values
  bool ext_drawBuffers = false; // set to use draw buffers
  bool ext_shaderTextureLOD = false; // set to use shader texture LOD
  */

  // When rendered geometry doesn't include these attributes but the material does,
  // use these default values in WebGL. This avoids errors when buffer data is missing.
  math::Vector3 default_color = {1, 1, 1};
  math::Vector2 default_uv = {0.0f, 0.0f};
  math::Vector2 default_uv2 = {0.0f, 0.0f};

  std::string index0AttributeName;

  ShaderMaterial(Shader &shader, Side side, bool depthTest, bool depthWrite, bool fog)
     : vertexShader(shader.vertexShader()), fragmentShader(shader.fragmentShader()),
       uniforms(shader.uniforms())
  {
    this->depthTest = depthTest;
    this->depthWrite = depthWrite;
    this->_fog = fog;
    this->_side = side;
  }

  ShaderMaterial(bool morphTargets, bool skinning) : Material(), uniforms({})
  {
    unsigned linewidth = 1;

    this->_morphTargets = morphTargets;
    this->_skinning = skinning;

    wireframe = false;
    wireframeLineWidth = 1;

    fog = false; // set to use scene fog
    lights = false; // set to use scene lights
  }

public:
  using Ptr = std::shared_ptr<ShaderMaterial>;
  static Ptr make(bool morphTargets, bool skinning) {
    return Ptr(new ShaderMaterial(morphTargets, skinning));
  }
  static Ptr make(Shader &shader, Side side, bool depthTest, bool depthWrite, bool fog) {
    return Ptr(new ShaderMaterial(shader, side, depthTest, depthWrite, fog));
  }
};

}
#endif //THREE_QT_SHADERMATERIAL_H