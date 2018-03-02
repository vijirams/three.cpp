//
// Created by byter on 12/14/17.
//

#ifndef THREEPPQ_QUICK_SPHERE_H
#define THREEPPQ_QUICK_SPHERE_H

#include <threepp/quick/scene/Scene.h>
#include <threepp/geometry/Sphere.h>
#include <threepp/material/MeshBasicMaterial.h>
#include <threepp/material/MeshLambertMaterial.h>
#include <threepp/objects/Mesh.h>

namespace three {
namespace quick {

class Sphere : public ThreeQObject
{
  Q_OBJECT
  Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
  Q_PROPERTY(unsigned widthSegments READ widthSegments WRITE setWidthSegments NOTIFY widthSegmentsChanged)
  Q_PROPERTY(unsigned heightSegments READ heightSegments WRITE setHeightSegments NOTIFY heightSegmentsChanged)

  qreal _radius = 1;
  unsigned _widthSegments=1, _heightSegments=1;

  //MeshCreatorG<geometry::Sphere> _creator {"sphere"};
  MeshCreator::Ptr _creator;

protected:
  three::Object3D::Ptr _create(Scene *scene) override
  {
    switch(_geometryType) {
      case Three::DefaultGeometry: {
        auto creator = MeshCreatorG<geometry::Sphere>::make("plane");
        creator->set(geometry::Sphere::make(_radius, _widthSegments, _heightSegments));
        _creator = creator;
        break;
      }
      case Three::BufferGeometry: {
        auto creator = MeshCreatorG<geometry::buffer::Sphere>::make("plane");
        creator->set(geometry::buffer::Sphere::make(_radius, _widthSegments, _heightSegments));
        _creator = creator;
        break;
      }
    }

    material()->identify(*_creator);

    return _creator->getMesh();
  }

  void updateMaterial() override {
    material()->identify(*_creator);
  }

public:
  Sphere(QObject *parent = nullptr) : ThreeQObject(parent) {}

  qreal radius() {return _radius;}
  unsigned widthSegments() const {return _widthSegments;}
  unsigned heightSegments() const {return _heightSegments;}

  void setRadius(qreal radius) {
    if(_radius != radius) {
      _radius = radius;
      emit radiusChanged();
    }
  }
  void setWidthSegments(unsigned widthSegments) {
    if(_widthSegments != widthSegments) {
      _widthSegments = widthSegments;
      emit widthSegmentsChanged();
    }
  }
  void setHeightSegments(unsigned heightSegments) {
    if(_heightSegments != heightSegments) {
      _heightSegments = heightSegments;
      emit heightSegmentsChanged();
    }
  }

signals:
  void widthSegmentsChanged();
  void heightSegmentsChanged();
  void radiusChanged();
};

}
}

#endif //THREEPPQ_QUICK_SPHERE_H
