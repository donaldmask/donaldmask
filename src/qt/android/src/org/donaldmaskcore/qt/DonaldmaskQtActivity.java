package org.donaldmaskcore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class DonaldmaskQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File donaldmaskDir = new File(getFilesDir().getAbsolutePath() + "/.donaldmask");
        if (!donaldmaskDir.exists()) {
            donaldmaskDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
