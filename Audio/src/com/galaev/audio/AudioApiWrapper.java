package com.galaev.audio;

import java.io.*;

/**
 * Wraps Windows Core Audio Api.
 */
public class AudioApiWrapper {

    public static final String DLL_64 = "audiojni64.dll";
    public static final String DLL_32 = "audiojni32.dll";

    static {
        String name;
        String bitness = System.getProperty("sun.arch.data.model");
        if (bitness.equals("32")) {
            name = DLL_32;
        } else {
            name = DLL_64;
        }
        String outPath = System.getProperty("java.io.tmpdir") + "audiojni"  + File.separator + name;
        try {
            System.load(outPath);
        } catch (UnsatisfiedLinkError e) {
            loadDllFromJar(name, outPath);
        }
    }

    private static void loadDllFromJar(String name, String outPath) {
        try{
            InputStream in = AudioApiWrapper.class.getResourceAsStream("/native/" + name);
            File tempPath = new File(System.getProperty("java.io.tmpdir") + "audiojni");
            if (! tempPath.exists()) {
                tempPath.mkdir();
            }
            File fileOut = new File(outPath);
            if (fileOut.exists()) {
                fileOut.delete();
            }
            fileOut.createNewFile();
            FileOutputStream out = new FileOutputStream(fileOut);

            // writing
            byte[] buffer = new byte[1024];
            int read;
            while((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            out.close();
            System.load(fileOut.toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static native boolean getMute();

    public static native void setMute(boolean mute);

    public static native float getVolumeLevel();

    public static native void setVolumeLevel(float newVolume);

    public static native float getBassLevel();

    public static native void setBassLevel(float newBass);

    public static native float getTrebleLevel();

    public static native void setTrebleLevel(float newTreble);

    public static native float getBassMin();

    public static native float getBassMax();

    public static native float getBassStepping();

    public static native float getTrebleMin();

    public static native float getTrebleMax();

    public static native float getTrebleStepping();
}
