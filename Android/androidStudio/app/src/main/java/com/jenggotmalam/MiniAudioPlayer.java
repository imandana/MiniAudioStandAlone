package com.jenggotmalam;

import java.util.*;  

import android.app.Activity;
import android.content.res.AssetManager;
import android.util.Log;

public class MiniAudioPlayer {

	static { System.loadLibrary("androidAPI"); }
	private Map<String, Integer> musicList;
	
	public Thread audioThread;
	private static final String TAG = "MiniAudioJava";

	/// Temporary, cannot be decreased dynamically
	private int indexMusic = 0;
	private ArrayList<Integer> reservedPos;
	
    public MiniAudioPlayer(Activity activity) {

		Log.v(TAG,  activity.getFilesDir().getPath() );
		InitAssetManagerMini(activity.getResources().getAssets() , activity.getFilesDir().getPath() );
		
		Log.v(TAG, "InitMiniaudio");
		InitMiniaudio();
		
		musicList = new HashMap<String, Integer>();
		reservedPos = new ArrayList<Integer>();
		
		
		audioThread = new Thread(new Runnable() {

						@Override
						public void run() {
							
							Log.v(TAG, "In Thread StartThreadMiniaudio();");
							StartThreadMiniaudio();
						 }
					 });
    }

	public void AddMusicStreamToPlay(String pathName) // 
	{
		if( indexMusic >= 12 ) // For now harddoced
			return;
			
		Log.v(TAG, "AddMusicStreamToPlay(String pathName)");
		
		if(reservedPos.size() > 0)
		{
			musicList.put(pathName, reservedPos.get( 0 ) );
			reservedPos.remove( 0 );
		}
		else
		{
			musicList.put(pathName, indexMusic);
		}
		indexMusic++;
		
		Log.v(TAG, "AddMusicStream( pathName );");
		AddMusicStream( pathName );

	}
	
	public void RemoveMusicStreamFromPlay(String pathName) // 
	{
		int pos = musicList.get( pathName );
		musicList.remove( pos );
		
		reservedPos.add( pos );
		
		indexMusic--;
		
		Log.v(TAG, "RemoveMusicStream( pos );");
		RemoveMusicStream( pos );
	}
	
	public void SetMusicVolumeOf(String pathName, float vol)
	{
		int pos = musicList.get( pathName );
		
		SetVolumeForMusic(pos, vol);
	}
	
	public void StartAudioThread()
	{
		if( !audioThread.isAlive() )
		{
			audioThread.start();
			return;
		}
	}
	
	public void SetPitchAllAudio(float pitch)
	{
		SetPitchAllMusic( pitch );	
	}
	
	public void StopAllAudio()
	{
		StopMiniaudio();	
	}
			
	public void PauseAllAudio()
	{
		PauseMiniaudio();	
	}
					
	public void ResumeAllAudio()
	{
		ResumeMiniaudio();	
	}
		
	public void PlayAllAudio()
	{
		// restart playing
		PlayMiniaudio();	
	}
	
    public void onDestroy() {
		
		SetIsClosed( 1 );
		CleanResource();
		
	}
	
	
	private native void InitAssetManagerMini(AssetManager mgr, String path);

	public native void SetIsClosed(int value);
	public native void AddMusicStream(String pathName);
	public native void RemoveMusicStream(int pos);
	public native void CleanResource();
	public native void InitMiniaudio();
	public native void PlayMiniaudio();
	
	public native void SetPitchAllMusic(float pitch);
	
	public native void StopMiniaudio();
	public native void PauseMiniaudio();
	public native void ResumeMiniaudio();
	
	public native void SetVolumeForMusic(int pos, float vol);
	
	public native void StartThreadMiniaudio();

	
}

