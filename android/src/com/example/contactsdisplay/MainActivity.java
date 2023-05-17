package com.example.contactsdisplay;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import java.util.ArrayList;

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
import android.net.Uri;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MainActivity extends QtActivity{

    public native void displayContacts(String jsonContacts, long pointer);
    public native void getUpdatedContacts(String jsonContacts, long pointer);
    public native void removeContact(String jsonContacts, long pointer);

    public long pointer;
    String lastUpdateTime = "0";
    int numOfContacts = 0;
    int contactListEmpty = 1;

    ContactObserver contactObserver = new ContactObserver(null);

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public void test(long ptr){
          pointer = ptr;

          if (checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED ||
              checkSelfPermission(Manifest.permission.WRITE_CONTACTS) != PackageManager.PERMISSION_GRANTED
          ){
               requestPermissions(new String[]{Manifest.permission.READ_CONTACTS, Manifest.permission.WRITE_CONTACTS}, 100);
           }
          else {
                if(contactListEmpty == 1 )
                    writeContacts();

                else
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
                numOfContacts = cursor.getCount();


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
        cursor.close();

        displayContacts(contactsArray.toString(), pointer);
    }

    public void writeContacts(){
        int i;
        for( i = 0; i < 1000; i++){
            ArrayList<ContentProviderOperation> operations = new ArrayList<>();
            operations.add(ContentProviderOperation.newInsert(ContactsContract.RawContacts.CONTENT_URI)
            .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
            .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
            .build());

            operations.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
            .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
            .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)
            .withValue(ContactsContract.CommonDataKinds.StructuredName.DISPLAY_NAME, "Te"+i+"t").build());


            operations.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
            .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID, 0)
            .withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
            .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, "054"+i+"685966884").build());
            try {
                getContentResolver().applyBatch(ContactsContract.AUTHORITY, operations);

            }
            catch(Exception ex){

            }
        }

        contactListEmpty = 0;
        readContacts();
    }

    @Override
       public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
           if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                readContacts();
           }
           else if(grantResults[1] == PackageManager.PERMISSION_GRANTED){
                writeContacts();
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
               //contact modified
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
                   getUpdatedContacts(contactsArray.toString(), pointer);
               }

           }
           catch(JSONException ex){}

            String[] newPROJECTION = new String[]{
                            ContactsContract.DeletedContacts.CONTACT_ID,
                            ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP
                    };
            final String delContactWhere = ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP + ">?";
            final String[] delContactArguments = { lastUpdateTime };
            final String delContactsOrderBy = ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP + " DESC";

            Cursor newCursor = contentResolver.query(ContactsContract.DeletedContacts.CONTENT_URI,newPROJECTION,delContactWhere,delContactArguments, delContactsOrderBy);

            String contact_id;



            try{
                //contact deleted
                if(newCursor.moveToFirst()){
                    JSONArray deletedContactsArray = new JSONArray();

                    do {

                        contact_id = newCursor.getString(newCursor.getColumnIndex(ContactsContract.DeletedContacts.CONTACT_ID));


                        if(newCursor.isFirst()){
                            lastUpdateTime = newCursor.getString(newCursor.getColumnIndex(ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP));
                        }

                        JSONObject deletedContact = new JSONObject();
                        deletedContact.put("contact_id", contact_id);

                        deletedContactsArray.put(deletedContact);


                    }while(newCursor.moveToNext());
                    removeContact(deletedContactsArray.toString(), pointer);

                }

            }
            catch(Exception ex){}


            cursor.close();
            newCursor.close();

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
