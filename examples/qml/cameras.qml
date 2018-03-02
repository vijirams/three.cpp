import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import three.quick 1.0

Window {
    id: mainWindow
    minimumWidth: 1280
    minimumHeight: 1024
    visible: true
    color: "gray"

    OptionsMenu {
        anchors.top: parent.top
        anchors.left: parent.left
        color: "transparent"
        width: 250
        height: 200
        z: 2

        BoolChoice {
            value: false
            name: value ? "Orthographic" : "Perspective"
            onValueChanged: {
                threeD.activeCamera = value ? orthoCamera : perspectiveCamera
            }
        }
    }

    ThreeD {
        id: threeD
        anchors.fill: parent
        focus: true
        antialias: true
        autoClear: false

        property real frustumSize: 600
        property int frameCount: 0
        property Camera activeCamera: perspectiveCamera
        property real aspect: width / height

        Scene {
            id: scene

            Sphere {
                id: sphere
                type: Three.BufferGeometry
                radius: 100
                widthSegments: 16
                heightSegments: 8

                material: MeshBasicMaterial {
                    color: "#ffffff"
                    wireframe: true
                }

                Sphere {
                    id: sphere2
                    type: Three.BufferGeometry
                    radius: 50
                    widthSegments: 16
                    heightSegments: 8
                    position.y: 150

                    material: MeshBasicMaterial {
                        color: "#00ff00"
                        wireframe: true
                    }
                }
            }

            Points {
                id: particles
                material.color: "#888888"

                Component.onCompleted: {
                    for ( var i = 0; i < 10000; i ++ ) {

                        var vertex = Qt.vector3d(
                                Three.randFloatSpread( 2000 ),
                                Three.randFloatSpread( 2000 ),
                                Three.randFloatSpread( 2000 ));

                        particles.addPoint( vertex );
                    }
                }
            }

            PerspectiveCamera {
                id: defaultCamera
                fov: 50
                aspect: 0.5 * threeD.aspect
                near: 1
                far: 10000
                position: "0,0,2500"
            }

            Group {
                id: cameraRig
                Sphere {
                    id: sphere3
                    type: Three.BufferGeometry
                    radius: 5
                    widthSegments: 16
                    heightSegments: 8
                    position.z: 150

                    material: MeshBasicMaterial {
                        color: "#0000ff"
                        wireframe: true
                    }
                }

                PerspectiveCamera {
                    id: perspectiveCamera
                    fov: 50
                    aspect: 0.5 * threeD.aspect
                    near: 150
                    far: 1000
                    rotation.y: Math.PI

                    helper.visible: true
                }

                OrthographicCamera {
                    id: orthoCamera
                    left: 0.5 * threeD.frustumSize * threeD.aspect / - 2
                    right: 0.5 * threeD.frustumSize * threeD.aspect / 2
                    top: threeD.frustumSize / 2
                    bottom: threeD.frustumSize / - 2
                    near: 150
                    far: 1000
                    rotation.y: Math.PI

                    helper.visible: true
                }
            }
        }

        onGeometryChanged: {
            defaultCamera.updateProjectionMatrix()
            perspectiveCamera.updateProjectionMatrix()
            orthoCamera.updateProjectionMatrix()
        }

        animate: function() {
            if((frameCount % 2) === 1) {
                threeD.viewport = Qt.rect(threeD.width/2,0,threeD.width/2,threeD.height);
                scene.camera = defaultCamera
            }
            else {
                var r = Date.now() * 0.0005;

                sphere.position.x = 700 * Math.cos( r );
                sphere.position.z = 700 * Math.sin( r );
                sphere.position.y = 700 * Math.sin( r );

                sphere2.position.x = 70 * Math.cos( 2 * r );
                sphere2.position.z = 70 * Math.sin( r );

                if (activeCamera === perspectiveCamera) {

                    perspectiveCamera.fov = 35 + 30 * Math.sin( 0.5 * r )
                    perspectiveCamera.far = sphere.position.length()
                    perspectiveCamera.updateProjectionMatrix()

                    perspectiveCamera.helper.update()
                    perspectiveCamera.helper.visible = true
                    orthoCamera.helper.visible = false;
                }
                else {

                    orthoCamera.far = sphere.position.length()
                    orthoCamera.updateProjectionMatrix()

                    orthoCamera.helper.update()
                    orthoCamera.helper.visible = true
                    perspectiveCamera.helper.visible = false
                }

                threeD.viewport = Qt.rect(0,0,threeD.width/2,threeD.height);
                scene.camera = activeCamera
            }

            cameraRig.lookAt( sphere.position );

            clear();
            frameCount++
        }
    }
}
