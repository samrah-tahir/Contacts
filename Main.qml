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
}





