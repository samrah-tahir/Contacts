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
            color: "#f0f1f2"

            MouseArea {
                anchors.fill: parent
                onClicked: console.log(contactsid.data(0,"contactName"))
            }

            ListView {
                    anchors.fill: parent
                    focus: true
                    anchors.margins: 20
                    model: ContactModel{}
                    spacing: 3
                    clip: true
                    header: Rectangle {
                        width: root.width;
                        height: 30

                        Text {
                            anchors.fill: parent
                            text: "Contacts"
                            font.bold: true
                        }
                    }

                    delegate: Rectangle {
                        width: root.width

                        height: 40
                        color: "white"
                        anchors.margins: 5

                            Text {
                                id: contactName
                                width: parent.width - 5
                                height: 20
                                text: model.contactName
                            }
                            Text {
                                anchors.top: contactName.bottom
                                width: parent.width - 5
                                height: 20
                                text: model.contactNumber
                                font.pointSize: 12
                            }
                        }

                        section.property: "contactName"
                        section.delegate: sectionDelegate
                        section.criteria: ViewSection.FirstCharacter
                    }

            Component {
                    id: sectionDelegate

                    Rectangle {
                        id: sectionWrapper
                        required property string section
                        width: ListView.view ? ListView.view.width : 0
                        height: 20
                        color: "white"

                        Text {
                            text: sectionWrapper.section
                            color: "grey"
                            font.pixelSize: 12
                        }



                    }
                }


                }

        }





