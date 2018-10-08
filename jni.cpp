
#include <jni.h>

#include <android/log.h>

#include <string.h>

#include <unistd.h>

#include "headers.h"
#include "main.h"

#include <time.h>

#include "archive.h"
#include "im_color.h"
#include "im_gen.h"
#include "im_mip.h"
#include "im_tex.h"
#include "im_image.h"
#include "im_png.h"
#include "pakfile.h"

#define DR_PCX_IMPLEMENTATION
#include "dr_pcx.h"

#define JAVA_FUNC(x) Java_com_opentouchgaming_androidcore_PakExt_##x

#define EXPORT_ME __attribute__ ((visibility("default")))

extern "C"
{


    jint EXPORT_ME  JAVA_FUNC(extract) ( JNIEnv* env, jobject thiz, jstring pakFile, jstring file, jint options, jstring outFile )
    {
       // chdir("/sdcard/wad");

    	const char *pakFile_s = (char *)(env)->GetStringUTFChars( pakFile, 0);
        const char *file_s = (char *)(env)->GetStringUTFChars( file, 0);
        const char *outFile_s = (char *)(env)->GetStringUTFChars( outFile, 0);

        LOGI("PAK file: %s, Looking for %s, Output %s", pakFile_s, file_s, outFile_s);


		COL_SetPalette( (game_kind_e)options );

		if (! PAK_OpenRead(pakFile_s))
		{
			FatalError("Cannot open PAK file: %s\n", pakFile_s);
			return -1;
		}

		int num_files = PAK_NumEntries();

		int skipped  = 0;
		int failures = 0;

		bool found = false;
		for (int i = 0; i < num_files; i++)
		{
			const char *name = PAK_EntryName(i);

			//printf("Unpacking %d/%d : %s\n", i+1, num_files, name);

			if( !strcmp( name, file_s ))
			{
				if (! ARC_ExtractOneFile(i, name, outFile_s))
				{
					LOGI("Failed to extract");
					return -1;
				}
				else
				{
					LOGI("Extracted from PAK");
					// convert PCX files from quake 2
					const char *dot = strrchr( name, '.');
					if (dot && !strcmp(dot, ".pcx"))
					{
						LOGI("Is pcx file");
				        int width;
	                    int height;
	                    int components;
	                    drpcx_uint8* pImageData = drpcx_load_file( outFile_s, DRPCX_FALSE, &width, &height, &components, 0);
	                    if (pImageData == NULL) {
	                        // Failed to load image.
	                    }
	                    else
	                    {
	                        LOGI("Loaded PCXL %d %d %d", width, height, components);
	                        rgb_image_c *raw = new rgb_image_c( width, height);

	                        for (int y = 0; y < height; y++)
	                            for (int x = 0; x < width;  x++)
	                            {
	                                byte *pix = &pImageData[(y*width + x) * components];

	                                raw->PixelAt(x, y) = MAKE_RGB(*pix, *pix+1, *pix+2);
	                            }

	                        FILE *fp = fopen( outFile_s, "wb");

	                        PNG_Save( fp, raw  );

	                        fclose(fp);
	                        delete raw;
	                        drpcx_free(pImageData);
	                        found = true;
	                    }
					}
					else
					{
						found = true;
					}
				}
				break;
			}
		}

        env->ReleaseStringUTFChars( pakFile, pakFile_s);
        env->ReleaseStringUTFChars( file, file_s);
        env->ReleaseStringUTFChars( outFile, outFile_s);

        if( !found )
			return -1;
		else
			return 0;
    }
}

