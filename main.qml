import QtQuick 2.7

import QtQuick.Window 2.2

import QtQuick.Controls 2.2
//import QmlMainwindow 1.0

ApplicationWindow  {
    id: mainwindow
    width: 600
    height: 480
    color: "white"
    visible: true
    Rectangle
    {
        width: parent.width
        height: parent.height
        id: root
        color: "#213854"
        //   flags:Qt.FramelessWindowHint
        Component.onCompleted: {
           // console.log("KKKKKKK")
          //  QmlMainwindow.aa();
        }

        Rectangle{
            id: start
            width: parent.width/5
            height: width
            radius: height/2
            color: "#379FDB"
            anchors.verticalCenter: root.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: width

            Text {
                id: mstart
                text: qsTr("Start")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: parent.height/4
                font.family: "微软雅黑"

            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    start.color = "red"
                    QmlMainwindow.cameraStart();
                    //启动摄像头进行录制

                }
                onReleased: {
                    start.color = "#379FDB"
                }
            }
        }

        Rectangle{
            id: stop
            width: parent.width/5
            height: width
            radius: height/2
            color: "#379FDB"
            anchors.verticalCenter: root.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: width

            Text {
                id: stopText
                text: qsTr("Stop")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: parent.height/4
                font.family: "微软雅黑"

            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    stop.color = "red"
                    //停止录制



                }
                onReleased: {
                    stop.color = "#379FDB"
                 //   var saveFile = Qt.createComponent("SaveVideo.qml").createObject(mainwindow);
                    QmlMainwindow.cameraStop()


                }
            }
        }
    }


}


