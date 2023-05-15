import QtQuick
import QtQuick.Window
import Contactspackage


Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Contact Book")

//    Contacts{
//        id: contactsid
//    }

    Rectangle {
            anchors.fill: parent
            color: "pink"

            MouseArea {
                anchors.fill: parent
                onClicked: console.log(contactsid.data(0,"contactName"))
            }

            ListView {
                    anchors.fill: parent
                    anchors.margins: 20

                    focus: true

                    model: ContactModel{}
                    delegate: Rectangle {
                        width: root.width
                        height: 40
                        color: "white"


                            Text {
                                id: contactName
                                width: parent
                                height: 20
                                text: model.contactName
                            }
                            Text {
                                anchors.top: contactName.bottom
                                width: parent
                                height: 20
                                text: model.contactNumber
                            }
                        }
                    }

                    clip: true
                }

        }





