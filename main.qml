import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// import the VTK module
import VTKPLUS 9.1
ApplicationWindow {
    title: qsTr("VTK QtQuick App")
    width: 800
    height: 600
    color: palette.window
    visible:  true
    SystemPalette {
      id: palette
      colorGroup: SystemPalette.Active
    }
    Text {
        id: text1
        text: qsTr("Hello")
        color: "white"
    }
    VTKRenderWindow {
        objectName: "RenderWindowQt"
      id: vtkwindow
      width: 400
      height: 400
    }

    VTKRenderItem {
        objectName: "ConeView"
//        x: 0
//        y: 0
//        width: 200
//        height: 200
        anchors.fill: parent
        renderWindow: vtkwindow
        focus: true
    }
//    VTKRenderItem {
//        objectName: "ConeView2"
//        x: 0
//        y: 200
//        width: 200
//        height: 200
////        anchors.fill: parent
//        renderWindow: vtkwindow
//        focus: true
//    }
}
