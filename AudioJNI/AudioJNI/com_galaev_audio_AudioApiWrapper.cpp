#include <jni.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <devicetopology.h>
#include <endpointvolume.h>

IAudioEndpointVolume* getIAudioEndpointVolume() {
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	return endpointVolume;
}


IDeviceTopology* getHardwareTopology() {
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IDeviceTopology *endpointTopology = NULL;
	defaultDevice->Activate(__uuidof(IDeviceTopology), CLSCTX_ALL, NULL, (LPVOID *)&endpointTopology);
	IConnector *endpointConnector = NULL;
	IConnector *hardwareConnector = NULL;
	endpointTopology->GetConnector(0, &endpointConnector);
	endpointConnector->GetConnectedTo(&hardwareConnector);
	IPart *hcPart = NULL;
	hardwareConnector->QueryInterface(__uuidof(IPart), (LPVOID *)&hcPart);
	IDeviceTopology *hardwareTopology = NULL;
	hcPart->GetTopologyObject(&hardwareTopology);

	return hardwareTopology;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_galaev_audio_AudioApiWrapper_getMute
(JNIEnv *env, jclass jc) {
	BOOL mute;
	CoInitialize(NULL);
	IAudioEndpointVolume* endpointVolume = getIAudioEndpointVolume();
	endpointVolume->GetMute(&mute);
	endpointVolume->Release();
	CoUninitialize();
	return mute;
}

extern "C"
JNIEXPORT void JNICALL Java_com_galaev_audio_AudioApiWrapper_setMute
(JNIEnv *env, jclass jc, jboolean mute) {
	CoInitialize(NULL);
	IAudioEndpointVolume* endpointVolume = getIAudioEndpointVolume();
	endpointVolume->SetMute(mute, NULL);
	endpointVolume->Release();
	CoUninitialize();
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getVolumeLevel
(JNIEnv *env, jclass jc) {
	jfloat currentVolume = 0;
	CoInitialize(NULL);
	IAudioEndpointVolume* endpointVolume = getIAudioEndpointVolume();
	endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	endpointVolume->Release();
	CoUninitialize();
	return currentVolume;
}

extern "C"
JNIEXPORT void JNICALL Java_com_galaev_audio_AudioApiWrapper_setVolumeLevel
(JNIEnv *env, jclass jc, jfloat newVolume){
	CoInitialize(NULL);
	IAudioEndpointVolume* endpointVolume = getIAudioEndpointVolume();
	endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
	endpointVolume->Release();
	CoUninitialize();
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getBassLevel
(JNIEnv *env, jclass cl){
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioBass *bass = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioBass), (void**)&bass);
		if (bass != NULL) {
			float level = NULL;
			bass->GetLevel(0, &level);
			return level;
		}
	}
	return -1;
	CoUninitialize();
}

extern "C"
JNIEXPORT void JNICALL Java_com_galaev_audio_AudioApiWrapper_setBassLevel
(JNIEnv *env, jclass cl, jfloat level){
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioBass *bass = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioBass), (void**)&bass);
		if (bass != NULL) {
			bass->SetLevelUniform(level, NULL);
		}
	}
	CoUninitialize();
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getTrebleLevel
(JNIEnv *env, jclass cl){
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioTreble *treble = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioTreble), (void**)&treble);
		if (treble != NULL) {
			float level = NULL;
			treble->GetLevel(0, &level);
			return level;
		}
	}
	return -1;
	CoUninitialize();
}

extern "C"
JNIEXPORT void JNICALL Java_com_galaev_audio_AudioApiWrapper_setTrebleLevel
(JNIEnv *env, jclass cl, jfloat level){
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioTreble *treble = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioTreble), (void**)&treble);
		if (treble != NULL) {
			treble->SetLevelUniform(level, NULL);
		}
	}
	CoUninitialize();
}

jfloat getBassLevelRange(bool min, bool max) {
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioBass *bass = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioBass), (void**)&bass);
		if (bass != NULL) {
			float minlevel = 0, maxlevel = 0, step = 0;
			bass->GetLevelRange(0, &minlevel, &maxlevel, &step);
			if (min) {
				return minlevel;
			} else if (max) {
				return maxlevel;
			} else {
				return step;
			}
		}
	}
	return -1;
	CoUninitialize();
}

jfloat getTrebleLevelRange(bool min, bool max) {
	CoInitialize(NULL);
	IDeviceTopology* hardwareTopology = getHardwareTopology();
	UINT count = NULL;
	hardwareTopology->GetSubunitCount(&count);
	for (int i = 0; i < count; ++i) {
		ISubunit *sub = NULL;
		IPart* hsPart = NULL;
		IAudioTreble *treble = NULL;
		hardwareTopology->GetSubunit(i, &sub);
		sub->QueryInterface(__uuidof(IPart), (void**)&hsPart);
		hsPart->Activate(CLSCTX_ALL, __uuidof(IAudioTreble), (void**)&treble);
		if (treble != NULL) {
			float minlevel = 0, maxlevel = 0, step = 0;
			treble->GetLevelRange(0, &minlevel, &maxlevel, &step);
			if (min) {
				return minlevel;
			}
			else if (max) {
				return maxlevel;
			}
			else {
				return step;
			}
		}
	}
	return -1;
	CoUninitialize();

}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getBassMin
(JNIEnv *env, jclass cl){
	return getBassLevelRange(true, false);
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getBassMax
(JNIEnv *env, jclass cl){
	return getBassLevelRange(false, true);
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getBassStepping
(JNIEnv *env, jclass cl){
	return getBassLevelRange(false, false);
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getTrebleMin
(JNIEnv *env, jclass cl){
	return getTrebleLevelRange(true, false);
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getTrebleMax
(JNIEnv *env, jclass cl){
	return getTrebleLevelRange(false, true);
}

extern "C"
JNIEXPORT jfloat JNICALL Java_com_galaev_audio_AudioApiWrapper_getTrebleStepping
(JNIEnv *env, jclass cl){
	return getTrebleLevelRange(false, false);
}