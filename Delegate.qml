import QtQuick
import Contactspackage
import QtQml.Models

Package {
    id: contactsPackage

    Rectangle { Package.name: 'list'
        id: listDelegate
        width: root.width; height: 40; anchors.margins: 5
        color: "white"
        MouseArea {
            anchors.fill: listDelegate
            drag {
                target: listDelegate
                axis: Drag.XAxis
                minimumX: -292
                maximumX: 0
            }

            onPositionChanged: if(drag.active) {
                                   if(listDelegate.x == -292){
                                      contactmodel.removeRows(index, 1);
                                    }

                               }

            onClicked: {
                        detailDelegate.ListView.view.currentIndex = index;
                        if (root.state == 'inList') {
                            root.state = 'inDetail'
                            drag.target = null
                        } else {
                            root.state = 'inList'
                            drag.target = listDelegate
                        }
                }
            }
        states: [
            State {
                name: 'inList'; when: root.state == 'inList' || root.state == ''
                ParentChange { target: contactAvatar; parent: listDelegate}
                ParentChange {target: contactNameText; parent: listDelegate}
                ParentChange {target: contactNumberLabel; parent: listDelegate}
            },
            State {
                name: 'inDetail'; when: root.state == 'inDetail'
                ParentChange {target: contactNumberLabel; parent: detailDelegate; width: parent.width; height: 40}
                ParentChange {target: contactNameText; parent: detailDelegate; width: parent.width;height:  40}
                ParentChange {target: contactAvatar; parent: detailDelegate; width: parent.width; height:  250}

                PropertyChanges {target: backBtn; visible: true}
                PropertyChanges {target: contactAvatar; width: parent.width; height: 140; radius: 0; x: 0; y:0}
                PropertyChanges {target: contactNameText; width: parent.width;height: 40; x:0; y: 140; color: "white"}
                PropertyChanges {target: contactNumberLabel; anchors.topMargin: 20;width: parent.width; height: 40; x:0;y: 200; color: "white"}
                AnchorChanges {target: contactNameText; anchors.top: contactAvatar.bottom; anchors.left: parent.left}
                AnchorChanges {target: contactNumberLabel; anchors.top: contactNameText.bottom; anchors.left: parent.left}
            }

        ]

        transitions: [
                    Transition {
                        from: 'inList'; to: 'inDetail'
                        ParentAnimation {
                            NumberAnimation {
                                properties: 'x,y,width,height,opacity'
                                duration: 300
                                easing.type: Easing.OutQuart
                            }
                        }
                    }
                ]

    }
    Rectangle { Package.name: 'detail'
        id: detailDelegate
        width: root.width; height: root.height
        color: "#f0f1f2"
    }
//================================================================================

        Rectangle {
            id: contactAvatar
            x: 3
            width: 30; height: 30
            color:  Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
            radius: 50
            Rectangle{
                id: backBtn
                color: "transparent"
                width: 80; height: 20
                Text {
                    y: 0; x: 0
                    text: "<< BACK"
                }
                visible: false
                MouseArea {
                    anchors.fill: backBtn
                    onClicked: {
                                detailDelegate.ListView.view.currentIndex = index;
                                root.state = 'inList'
                            }
                    }
            }
            Text {
                text: contactName.charAt(0)
                anchors.centerIn: parent
                color: "white"
            }
        }

        Rectangle {
            id: contactNameText
            width: root.width - 33; height: 16;
            anchors.left: contactAvatar.right
            Text {text: contactName}
        }

        Rectangle {
            id: contactNumberLabel
            width: root.width - 33; height: 16
            anchors {top: contactNameText.bottom; left: contactAvatar.right}
            Text {text: contactNumber; font.pointSize: 12}
        }
}



