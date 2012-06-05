package net.itskewpie.freerdp;

import android.app.Activity;
import android.os.Bundle;

public class FreeRDPActivity extends Activity {
   /** Called when the activity is first created. */
   @Override
   public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        System.out.println("111111111111111111111");
        String b = test();
        System.out.println(b);
        
    	}
   
   static{	   
	   System.loadLibrary("freerdp");
	   }
   public native String test();
   
}
