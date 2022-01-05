import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.3
import QtQuick.Controls.Material 2.2
// import the VTK module
import VTKPLUS 9.1

ApplicationWindow {
    id: root
    minimumWidth: 1024
    minimumHeight: 700
    visible: true
    title: "QtVTK"

    Material.primary: Material.Indigo
    Material.accent: Material.LightBlue

    property bool isModelSelected: qtVTKItem.isModelSelected

    VTKRenderWindow {
        objectName: "RenderWindowQt"
        id: vtkwindow
        anchors.fill: parent
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

    Button {
        id: openFileButton
        text: "Open file"
        highlighted: isModelSelected
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 50
        onClicked: qtVTKItem.showFileDialog = true;

        ToolTip.visible: hovered
        ToolTip.delay: 1000
        ToolTip.text: "Open a 3D model into the canvas"
        background: Rectangle {
            color: parent.down ? "#bbbbbb" :
                                 (parent.hovered ? "#d6d6d6" : "#f6f6f6")
        }
    }

    ComboBox {
        id: representationCombobox
        visible: isModelSelected
        width: 200
        model: ["Points", "Wireframe", "Surface"]
        currentIndex: 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 40
        anchors.topMargin: 30
        onActivated: qtVTKItem.setModelsRepresentation(currentIndex);
    }

    Slider {
        id: opacitySlider
        visible: isModelSelected
        width: 200
        value: 1
        from: 0.1
        to: 1
        stepSize: 0.01
        anchors.left: parent.left
        anchors.top: representationCombobox.bottom
        anchors.leftMargin: 40
        anchors.topMargin: 30

        onValueChanged: qtVTKItem.setModelsOpacity(value);
    }

    Switch {
        id: gouraudInterpolationSwitch
        visible: isModelSelected
        text: "Gouraud interpolation"
        anchors.left: parent.left
        anchors.top: opacitySlider.bottom
        anchors.leftMargin: 40
        anchors.topMargin: 30

        onCheckedChanged: qtVTKItem.setGouraudInterpolation(checked);
    }

    SpinBox {
        id: modelColorR
        visible: isModelSelected
        value: 3
        from: 0
        to: 255
        //            onValueChanged: canvasHandler.setModelColorR(value);
        anchors.left: parent.left
        anchors.top: gouraudInterpolationSwitch.bottom
        anchors.leftMargin: 40
        anchors.topMargin: 30
    }

    SpinBox {
        id: modelColorG
        visible: isModelSelected
        value: 169
        from: 0
        to: 255
        //            onValueChanged: canvasHandler.setModelColorG(value);
        anchors.left: parent.left
        anchors.top: modelColorR.bottom
        anchors.leftMargin: 40
        anchors.topMargin: 25
    }

    SpinBox {
        id: modelColorB
        visible: isModelSelected
        value: 244
        from: 0
        to: 255
        //            onValueChanged: canvasHandler.setModelColorB(value);
        anchors.left: parent.left
        anchors.top: modelColorG.bottom
        anchors.leftMargin: 40
        anchors.topMargin: 25
    }

    Label {
        id: positionLabelX
        visible: isModelSelected
        text: "X: " /*+ canvasHandler.modelPositionX*/
        font.pixelSize: 12
        anchors.bottom: positionLabelY.top
        anchors.left: parent.left
        anchors.margins: 40
    }

    Label {
        id: positionLabelY
        visible: isModelSelected
        text: "Y: " /*+ canvasHandler.modelPositionY*/
        font.pixelSize: 12
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 40
    }


    FileDialog {
        id: openModelsFileDialog
        visible: qtVTKItem.showFileDialog
        title: "Import model"
        folder: shortcuts.documents
        nameFilters: ["Model files" + "(*.stl *.STL *.obj *.OBJ)", "All files" + "(*)"]

        onAccepted: {
            qtVTKItem.showFileDialog = false;
            qtVTKItem.openModel(fileUrl);
        }
        onRejected: {
            qtVTKItem.showFileDialog = false;
        }
    }
}
