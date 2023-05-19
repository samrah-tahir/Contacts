import QtQuick
import Contactspackage
import QtQml.Models

Package {
    id: contactsPackage

    Item { Package.name: 'list'
        id: listDelegate
        width: root.width; height: 40
    }
    Item { Package.name: 'detail'
        id: detailDelegate
        width: root.width - 20; height: root.height
    }
//=====================================================
    Rectangle {
        id: contact
        width: root.width; height: 40
        Item {
            id: itemCon

            Row{
                spacing: 5
                padding: 5
                Rectangle {
                    width: 30
                    height: 30
                    color:  "grey"
                    radius: 50

                    Text {
                        text: contactName.charAt(0)
                        anchors.centerIn: parent
                        color: "white"
                    }
                }

                Text {
                    id: contactNameText
                    text: contactName
                }
            }

        }

        MouseArea {
            anchors.fill: contact
            drag {
                target: contact
                axis: Drag.XAxis
                minimumX: -292
                maximumX: 0
            }

            onPositionChanged: if(drag.active) {
                                   if(contact.x == -292){
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
                                drag.target = contact
                            }
                    }
            }

    Rectangle {
        id: detailsCardView
        width: detailDelegate.ListView.view.width; height:root.height
        visible: false
        color: "#f7f8fa"
        y: -4

        Rectangle {
            id: backBtn
            width: 40
            height: 20
            border.color: "black"
            anchors.left: parent.left
            y: 5
            Text {text: "Back"; anchors.centerIn: parent}
        }

        Column {
            anchors {centerIn: parent; margins: 20}
            spacing: 10
            Rectangle {
                id: avatar
                y: 0
                width: 350
                height: 250
                color: "#dcdee0"

            }

            Rectangle {
                id: detailName
                color: "white"
                width: 350; height: 40
                Text { text: contactName}
            }

            Rectangle{
                id: detailNumber
                color: "white"
                width: 350; height: 40
                Text { text: contactNumber}
            }
        }
    }

    states: [
        State {
            name: 'inList'; when: root.state == 'inList' || root.state == ''
            ParentChange { target: contact; parent: listDelegate}
        },
        State {
            name: 'inDetail'; when: root.state == 'inDetail'
            ParentChange {
                target: contact; parent: detailDelegate
                width: detailDelegate.ListView.view.width; height: detailDelegate.height
                x: 0; y: 0
            }
            PropertyChanges {target: detailsCardView; visible: true}
        }
    ]

    transitions: [
                Transition {
                    from: 'inList'; to: 'inDetail'
                    ParentAnimation {
                        NumberAnimation {
                            properties: 'y'
                            to: 0
                            duration: 200
                           // easing.type: Easing.InQuart
                        }
                    }
                }
            ]
}
}

