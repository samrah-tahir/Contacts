import QtQuick
import QtQuick.Window
import Contactspackage


Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Contact Book")
    color: "#f0f1f2"

    property string name
    property string number

    ContactModel{
        id:contactmodel
    }
    ListView {
            anchors {fill: parent; margins: 20}
            spacing: 2
            clip: true
            model: contactmodel
            header: Rectangle {
                width: root.width; height: 30
                Text {
                    anchors.fill: parent
                    text: "Contacts"
                    font.bold: true
                }
            }
            delegate: Rectangle {
                id: contact
               // required property int index

                width: ListView.view.width; height: 40
                Text {
                    id: contactName
                    text: model.contactName
                }
                Text {
                    anchors.top: contactName.bottom
                    text: model.contactNumber
                    font.pointSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    drag {
                        target: contact
                        axis: Drag.XAxis
                        minimumX: -292
                        maximumX: 0
                    }
                    onClicked: {
                        detailsCard.visible ? detailsCard.visible = false : detailsCard.visible = true;
                        name = model.contactName; number = model.contactNumber
                    }
                    onPositionChanged: if(drag.active) {
                                           if(contact.x == -292){
                                              contactmodel.removeRows(index, 1);
                                            }
                                       }
                }
            }

            section.property: "contactName"
            section.delegate: Rectangle {
                id: sectionWrapper

                required property string section

                width: ListView.view ? ListView.view.width : 0
                height: 20
                color: "white"

                Text {
                    text: sectionWrapper.section
                    color: "grey"
                    font.pointSize: 12
                }
            }
            section.criteria: ViewSection.FirstCharacter
    }

    Rectangle {
        id: detailsCard
        width: parent.width; height: parent.height/2
        visible: false
        color: "#f7f8fa"

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Row {
            spacing: 20
            Text {
                id: detailName
                text: name ? name : "N/A"

            }

            Text {
                id: detailNumber
                text: number ? number : "N/A"
            }
        }
    }
}





