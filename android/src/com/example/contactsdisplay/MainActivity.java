package com.example.contactsdisplay;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

import org.qtproject.qt.android.bindings.QtActivity;

import android.os.Bundle;
import android.Manifest;
import android.content.pm.PackageManager;
import android.util.Log;

import android.content.*;
import android.database.Cursor;
import android.database.ContentObserver;
import android.provider.*;
import android.os.Handler;

public class MainActivity extends QtActivity{

    public native void displayContacts(String jsonContacts, long pointer);
    public native void getUpdatedContacts(String jsonContacts, long pointer);

    public long pointer;
    String lastUpdateTime = "0";

    ContactObserver contactObserver = new ContactObserver(null);

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public void test(long ptr){
          pointer = ptr;

          if (checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED){
               requestPermissions(new String[]{Manifest.permission.READ_CONTACTS}, 100);
           }
          else {
                   readContacts();
          }


    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        getContentResolver().unregisterContentObserver(contactObserver);
    }

    public void readContacts(){
        ContentResolver contentResolver = getContentResolver();
        String[] PROJECTION = new String[]{
                        ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP,
                        ContactsContract.CommonDataKinds.Phone.CONTACT_ID,
                        ContactsContract.Contacts.DISPLAY_NAME,
                        ContactsContract.CommonDataKinds.Phone.NUMBER
                };
        final String contactsOrderBy = ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + " DESC";
        final String contactWhere = ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + ">?";
        final String[] contactArguments = { lastUpdateTime };
        contentResolver.registerContentObserver(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, true, contactObserver);
        Cursor cursor = contentResolver.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,PROJECTION,contactWhere,contactArguments,contactsOrderBy);


        JSONArray contactsArray = new JSONArray();


        String name;
        String number;
        String id;


        try{
            if(cursor.moveToFirst()){
                do {
                    name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
                    number = cursor.getString(cursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));

                    id = cursor.getString(cursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.CONTACT_ID));

                    if(cursor.isFirst()){
                        lastUpdateTime = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP));

                    }
                    JSONObject contact = new JSONObject();
                    contact.put("id", id);
                    contact.put("name", name);
                    contact.put("number", number);

                    contactsArray.put(contact);

                }while(cursor.moveToNext());
            }
        }
        catch(JSONException ex){}

        displayContacts(contactsArray.toString(), pointer);
    }

    @Override
       public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
           if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            readContacts();
           }
       }


   public void contactChanged(){
       ContentResolver contentResolver = getContentResolver();

       String[] PROJECTION = new String[]{
                       ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP,
                       ContactsContract.CommonDataKinds.Phone.CONTACT_ID,
                       ContactsContract.Contacts.DISPLAY_NAME,
                       ContactsContract.CommonDataKinds.Phone.NUMBER
               };

       final String contactsOrderBy = ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + " DESC";
       final String contactWhere = ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + ">?";
       final String[] contactArguments = { lastUpdateTime };

       Cursor cursor = contentResolver.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,PROJECTION,contactWhere,contactArguments,contactsOrderBy);

       JSONArray contactsArray = new JSONArray();


       String name;
       String number;
       String id;

       try{
           if(cursor.moveToFirst()){
               do {

                   name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
                   number = cursor.getString(cursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
                   id = cursor.getString(cursor.getColumnIndex(ContactsContract.CommonDataKinds.Phone.CONTACT_ID));
                   String time = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP));


                   if(cursor.isFirst()){

                       //last time the contact db was updated
                       lastUpdateTime = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP));
                   }


                   JSONObject contact = new JSONObject();
                   contact.put("id", id);
                   contact.put("name", name);
                   contact.put("number", number);

                   contactsArray.put(contact);

               }while(cursor.moveToNext());
           }

       }
       catch(JSONException ex){}

       getUpdatedContacts(contactsArray.toString(), pointer);

   }


   class ContactObserver extends ContentObserver {
        public ContactObserver(Handler handler){
            super(handler);
        }

        @Override
        public void onChange(boolean selfChange){
            contactChanged();
        }
   }
}
