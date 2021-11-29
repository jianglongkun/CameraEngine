#pragma once
#ifndef __CAMERA_ENGINE__H_
#define __CAMERA_ENGINE__H_

#include <windows.h>
#include <stdint.h>
#include <tchar.h>
#include <string>

#ifdef CAMERA_ENGINE_BUILD_DLL
#	define CAMERA_ENGINE_EXPORT __declspec(dllexport)
#else
#	define CAMERA_ENGINE_EXPORT __declspec(dllimport)
#endif

#ifdef UNICODE
typedef std::wstring TSTRING;
#else
typedef std::string TSTRING;
#endif

#pragma comment(lib,"VideoRecordCore.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msvcrtd.lib")

#ifdef __cplusplus
extern "C"
{
#endif
	//图像编码格式
	enum ImageFormat
	{
		IF_BMP,
		IF_JPEG,
		IF_TIFF,
		IF_PNG,
		IF_GIF,
	};

	//视频原始流数据格式
	enum StreamFormat
	{
		YV12 = 0,
		RGB24 = 1,
	};

	//镜头设备属性信息
	struct DeviceAttributeInfo
	{
		unsigned int w;//宽
		unsigned int h;//高
		StreamFormat format;//数据流格式
		int bitCount;//像素字节
		int fps;//帧率
	};

	//数据流接口参数
	struct StreamCallbackParameter
	{
		void *pUserObject;//用户对象
		unsigned char *pStream;//数据流
		unsigned int width;//宽
		unsigned int height;//高
		unsigned int length;//数据流长度
		StreamFormat format;//数据格式
		double timestamp;//时间戳
	};

	//镜头输出图像质量类型
	enum CameraImageQualityType
	{
		IMAGE_BRIGHTNESS=0,//亮度
		IMAGE_CONTRAST,//对比度
		IMAGE_HUE,//色调
		IMAGE_SATURATION,//饱和度
		IMAGE_SHARPNESS,//清晰度
		IMAGE_GAMMA,//伽马
		IMAGE_COLOR_ENABLE,//颜色
		IMAGE_WHITE_BALANCE,//白平衡
		IMAGE_BACKLIGHT_COMPENSATION,//背光补偿
		IMAGE_GAIN,//图像增益
	};

	struct CameraImageQualityValue
	{
		long cur;//当前
		long min;//最小
		long max;//最大
		long step;//步进
		long def;//默认
		long flag;//1:auto,2:manual
	};

	//图像叠层标签类型
	enum ImageOverlayMarkerType
	{
		OVERLAY_MARKER_IMAGE,//图像标签
		OVERLAY_MARKER_TEXT,//文字标签
	};

	//图像叠层标签信息
	struct ImageOverlayMarkerImageInfo
	{
		int x_pos;//x坐标
		int y_pos;//y坐标
		int w;//宽
		int h;//高
		TCHAR filePath[MAX_PATH];//图片路径
		//TSTRING filePath;
	};

	enum FontStyleType
	{
		FONT_STYLE_TYPE_NORMAL=0,//正常
		FONT_STYLE_TYPE_BOLD = 1,//加粗
		FONT_STYLE_TYPE_ITALIC=2,//斜体
		FONT_STYLE_TYPE_BOLD_ITALIC=3,//加粗斜体
		FONT_STYLE_TYPE_UNDERLINE=4,//下划线
		FONT_STYLE_TYPE_STRIKEOUT=8,//删除线
	};

	//图像叠层文字信息
	struct ImageOverlayMarkerTextInfo
	{
		int x_pos;//x坐标
		int y_pos;//y坐标
		int fontColor;//颜色
		short fontSize;//字体大小
		FontStyleType fontStyleType;//字体样式
		//TSTRING fontName;//字体名
		//TSTRING text;
		TCHAR fontName[MAX_PATH];
		TCHAR text[MAX_PATH];
	};

	enum PreviewRotateType
	{
		ROTATE_NONE_FLIP=0,//正常
		ROTATE_90_FLIP=1,//旋转90度
		ROTATE_180_FLIP=2,//旋转180度
		ROTATE_270_FLIP=3,//旋转270度

		ROTATE_NONE_X_FLIP,//X轴正常
		ROTATE_90_X_FLIP,//X轴90度翻转
		ROTATE_180_X_FLIP,//X轴180度翻转
		ROTATE_270_X_FLIP,//X轴270度翻转

		ROTATE_NONE_Y_FLIP= ROTATE_180_X_FLIP,
		ROTATE_90_Y_FLIP = ROTATE_270_X_FLIP,
		ROTATE_180_Y_FLIP = ROTATE_NONE_X_FLIP,
		ROTATE_270_Y_FLIP = ROTATE_90_X_FLIP,

		ROTATE_NONE_XY_FLIP = ROTATE_180_FLIP,
		ROTATE_90_XY_FLIP = ROTATE_270_FLIP,
		ROTATE_180_XY_FLIP = ROTATE_NONE_FLIP,
		ROTATE_270_XY_FLIP = ROTATE_90_FLIP,
	};

	/*
	struct ImageOverlayMarkerInfo
	{
		ImageOverlayMarkerType type;
		struct ImageOverlayMarkerImageInfo image;
		struct ImageOverlayMarkerTextInfo text;
	};
	*/
	//镜头原始数据流，不要同步处理改数据流，不然会影响底层的预览及视频录制的性能
	CAMERA_ENGINE_EXPORT typedef void(*StreamCallback)(StreamCallbackParameter *pStreamCallbackParameter);

	//������ͷ����
	//CAMERA_ENGINE_EXPORT void __stdcall CreateCameraEngine();

	//ɾ����ͷ����
	//CAMERA_ENGINE_EXPORT void __stdcall DeleteCameraEngine();

	//初始化
	CAMERA_ENGINE_EXPORT bool __stdcall Initialize();

	//释放初始化
	CAMERA_ENGINE_EXPORT void __stdcall Uninitialize();

	//获取当前机器上的镜头设备数量
	//返回值:返回设备数量，<0表示没有设备或者获取失败
	CAMERA_ENGINE_EXPORT int __stdcall GetDeviceCount();

	//根据索引返回设备名称，idx从0开始
	//返回值:返回NULL表示没有设备，或获取设备名称失败
	CAMERA_ENGINE_EXPORT TCHAR *__stdcall GetDeviceNameByIndex(int idx);

	//根据设备名称查找设备索引idx
	//返回值:-1:表示失败,>=0表示成功，返回设备索引ID
	CAMERA_ENGINE_EXPORT int GetDeviceIndexByName(TSTRING name);

	//根据idx索引打开相应的设备，idx索引从0开始
	CAMERA_ENGINE_EXPORT bool __stdcall OpenDeviceByIndex(int idx);

	//根据name设备名称打开相应的设备
	CAMERA_ENGINE_EXPORT bool __stdcall OpenDeviceByName(TSTRING name);

	//获取当前已打开设备的属性数量
	//返回值:<=0为失败，>0表示成功获取到数量
	CAMERA_ENGINE_EXPORT int __stdcall GetDeviceAttributeCount();

	//根据idx索引获取设备属性信息，idx索引从0开始
	CAMERA_ENGINE_EXPORT struct DeviceAttributeInfo __stdcall GetDeviceAttributeByIndex(int idx);

	//设置当前已打开的设备属性信息
	CAMERA_ENGINE_EXPORT bool __stdcall SetDeviceAttribute(struct DeviceAttributeInfo *pDeviceAttributeInfo);

	//设置当前已打开的设备分辨率
	CAMERA_ENGINE_EXPORT bool __stdcall SetResolution(int w, int h);

	//设置预览窗口句柄
	CAMERA_ENGINE_EXPORT void __stdcall SetPreviewHandle(HWND handle);

	//设置接收原始数据流的接口
	CAMERA_ENGINE_EXPORT void __stdcall SetStreamCallback(StreamCallback pStreamCallback, void *pUserObject);

	//移除接收原始数据流的接口
	CAMERA_ENGINE_EXPORT void __stdcall RemoveStreamCallback(StreamCallback pStreamCallback);

	//启动预览
	CAMERA_ENGINE_EXPORT bool __stdcall StartPreview();

	//停止预览
	CAMERA_ENGINE_EXPORT bool __stdcall StopPreview();

	//暂停预览
	CAMERA_ENGINE_EXPORT bool __stdcall PausePreview();

	//恢复预览
	CAMERA_ENGINE_EXPORT bool __stdcall ResumePreview();

	//启动录像
	//pRecordingFilePath:录像文件的存储路径及后缀格式
	//样本:D:\\record.avi
	//w:录像的宽
	//h:录像的高
	CAMERA_ENGINE_EXPORT bool __stdcall StartRecording(TCHAR *pRecordingFilePath,int w,int h);

	//停止录像
	CAMERA_ENGINE_EXPORT bool __stdcall StopRecording();

	//暂停录像
	CAMERA_ENGINE_EXPORT bool __stdcall PauseRecording();

	//恢复录像
	CAMERA_ENGINE_EXPORT bool __stdcall ResumeRecording();

	//采集一帧图像保存为图片
	//pImageFilePath:图片文件的存储路径及后缀格式
	//样本:D:\\image.jpg
	//format:图片格式类型
	CAMERA_ENGINE_EXPORT bool __stdcall CaptureImage(TCHAR *pImageFilePath, ImageFormat format);

	//获取设备的输出图像质量参数
	CAMERA_ENGINE_EXPORT bool __stdcall GetCameraImageQuality(CameraImageQualityType type, struct CameraImageQualityValue *value);

	//设置设备的输出图像质量
	CAMERA_ENGINE_EXPORT bool __stdcall SetCameraImageQuality(CameraImageQualityType type, long value);

	//获取叠层标签的数量
	//CAMERA_ENGINE_EXPORT int __stdcall GetOverlayMarkerCount();

	CAMERA_ENGINE_EXPORT int __stdcall GetOverlayMarkerImageCount();

	CAMERA_ENGINE_EXPORT bool __stdcall GetOverlayMarkerImageInfoByIndex(int idx, struct ImageOverlayMarkerImageInfo *pInfo);

	CAMERA_ENGINE_EXPORT bool __stdcall UpdateOverlayMarkerImageInfoByIndex(int idx, struct ImageOverlayMarkerImageInfo *pInfo);

	CAMERA_ENGINE_EXPORT int __stdcall AddOverlayMarkerImage(struct ImageOverlayMarkerImageInfo *pInfo);

	CAMERA_ENGINE_EXPORT bool __stdcall RemoveOverlayMarkerImageByIdx(int idx);

	CAMERA_ENGINE_EXPORT int __stdcall GetOverlayMarkerTextCount();

	CAMERA_ENGINE_EXPORT bool __stdcall GetOverlayMarkerTextInfoByIndex(int idx, struct ImageOverlayMarkerTextInfo *pInfo);

	CAMERA_ENGINE_EXPORT bool __stdcall UpdateOverlayMarkerTextInfoByIndex(int idx, struct ImageOverlayMarkerTextInfo *pInfo);

	CAMERA_ENGINE_EXPORT int __stdcall AddOverlayMarkerText(struct ImageOverlayMarkerTextInfo *pInfo);

	CAMERA_ENGINE_EXPORT bool __stdcall RemoveOverlayMarkerTextByIdx(int idx);

	/*
	//根据idx索引获取叠层标签的内容信息
	CAMERA_ENGINE_EXPORT bool __stdcall GetOverlayMarkerInfo(int idx, struct ImageOverlayMarkerInfo *pInfo);

	//根据idx索引来更新叠层标签的内容信息
	CAMERA_ENGINE_EXPORT bool __stdcall UpdateOverlayMarkerInfo(int idx, struct ImageOverlayMarkerInfo *pInfo);

	//添加一个叠层标签
	//返回值:-1：表示失败，成功返回id
	CAMERA_ENGINE_EXPORT int __stdcall AddOverlayMarker(struct ImageOverlayMarkerInfo *pInfo);

	//移除一个叠层标签
	CAMERA_ENGINE_EXPORT bool __stdcall RemoveOverlayMarker(int idx);
	*/
	//录像叠层标签是否可用
	CAMERA_ENGINE_EXPORT void __stdcall RecordOverlayMarkerEnable(bool enable);

	//录像叠层标签，时间显示是否可用
	//format:Y:H:S:M
	CAMERA_ENGINE_EXPORT void __stdcall RecordOverlayTimeMarkerEnable(bool enable,TSTRING *format=NULL);

	//设置预览图像旋转方向
	CAMERA_ENGINE_EXPORT void __stdcall SetPreviewRotateType(PreviewRotateType type);

	CAMERA_ENGINE_EXPORT PreviewRotateType __stdcall GetPreviewRotateType();

#ifdef __cplusplus
}
#endif

#endif