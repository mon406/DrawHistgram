#ifndef __INCLUDED_H_ImageHistgram__
#define __INCLUDED_H_ImageHistgram__

#include "main.h"


/* 入出力画像 */
Mat Image_hist;		// ヒストグラム画像

/* 関数宣言 */
void DrawHist(Mat& targetImg, Mat& dstHistImg);	// ヒストグラム計算
void DrawHist(Mat& targetImg, Mat& dstHistImg, int MaxCounter);	// ヒストグラム計算(高さ固定)

/* 関数 */
// ヒストグラム計算
void DrawHist(Mat& targetImg, Mat& dstHistImg) {
	dstHistImg = Mat::zeros(targetImg.size(), CV_8U);

	if (targetImg.channels() == 3) {
		Mat channels[3];
		int cha_index, ha_index;
		uchar Gray;
		for (int channel = 0; channel < 3; channel++) {
			channels[channel] = Mat(Size(targetImg.cols, targetImg.rows), CV_8UC1);
			for (int i = 0; i < targetImg.rows; i++) {
				for (int j = 0; j < targetImg.cols; j++) {
					cha_index = i * targetImg.cols * 3 + j * 3 + channel;
					ha_index = i * targetImg.cols + j;
					Gray = (uchar)targetImg.data[cha_index];
					channels[channel].data[ha_index] = Gray;
				}
			}
		}

		/* 変数宣言 */
		Mat R, G, B;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* 画素数を数える */
		calcHist(&channels[0], 1, 0, Mat(), B, 1, &hist_size, &hist_range);
		calcHist(&channels[1], 1, 0, Mat(), G, 1, &hist_size, &hist_range);
		calcHist(&channels[2], 1, 0, Mat(), R, 1, &hist_size, &hist_range);

		/* 確認（ヒストグラム高さ固定のため）*/
		int MAX_COUNT = 0;
		double Min_count[3], Max_count[3];
		for (int ch = 0; ch < 3; ch++) {
			if (ch == 0) { minMaxLoc(B, &Min_count[ch], &Max_count[ch]); }
			else if (ch == 1) { minMaxLoc(G, &Min_count[ch], &Max_count[ch]); }
			else if (ch == 2) { minMaxLoc(R, &Min_count[ch], &Max_count[ch]); }
			if (Max_count[ch] > MAX_COUNT) {
				MAX_COUNT = (int)Max_count[ch];
			}
		}
		//MAX_COUNT = 80000;

		/* ヒストグラム生成用の画像を作成 */
		Image_hist = Mat(Size(276, 320), CV_8UC3, Scalar(255, 255, 255));

		/* 背景を描画（見やすくするためにヒストグラム部分の背景をグレーにする） */
		for (int i = 0; i < 3; i++) {
			rectangle(Image_hist, Point(10, 10 + 100 * i), Point(265, 100 + 100 * i), Scalar(230, 230, 230), -1);
		}

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(R.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 255), 1, 8, 0);
			line(Image_hist, Point(10 + i, 200), Point(10 + i, 200 - (int)((float)(G.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 255, 0), 1, 8, 0);
			line(Image_hist, Point(10 + i, 300), Point(10 + i, 300 - (int)((float)(B.at<float>(i) / MAX_COUNT) * 80)), Scalar(255, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// 横軸10ずつラインを引く
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// 横軸50ずつ濃いラインを引く
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* ヒストグラム情報表示 */
		cout << "--- ヒストグラム情報 -------------------------" << endl;
		cout << " RGB画像" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "----------------------------------------------" << endl;

		/* ヒストグラム画像の出力 */
		Image_hist.copyTo(dstHistImg);
	}
	else if (targetImg.channels() == 1) {
		Mat channels;
		int ha_index;
		uchar Gray;
		channels = Mat(Size(targetImg.cols, targetImg.rows), CV_8UC1);
		for (int i = 0; i < targetImg.rows; i++) {
			for (int j = 0; j < targetImg.cols; j++) {
				ha_index = i * targetImg.cols + j;
				Gray = (uchar)targetImg.data[ha_index];
				channels.data[ha_index] = Gray;
			}
		}

		/* 変数宣言 */
		Mat GRAY;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* 画素数を数える */
		calcHist(&channels, 1, 0, Mat(), GRAY, 1, &hist_size, &hist_range);

		/* 確認（ヒストグラム高さ固定のため）*/
		int MAX_COUNT = 0;
		double Min_count, Max_count;
		minMaxLoc(GRAY, &Min_count, &Max_count);
		if (Max_count > MAX_COUNT) {
			MAX_COUNT = (int)Max_count;
		}
		//MAX_COUNT = 80000;

		/* ヒストグラム生成用の画像を作成 */
		Image_hist = Mat(Size(276, 120), CV_8UC3, Scalar(255, 255, 255));

		/* 背景を描画（見やすくするためにヒストグラム部分の背景をグレーにする） */
		rectangle(Image_hist, Point(10, 10), Point(265, 100), Scalar(230, 230, 230), -1);

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(GRAY.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// 横軸10ずつラインを引く
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// 横軸50ずつ濃いラインを引く
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* ヒストグラム情報表示 */
		cout << "--- ヒストグラム情報 ------------------------------" << endl;
		cout << " GrayScale画像" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "---------------------------------------------------" << endl;

		/* ヒストグラム画像の出力 */
		Image_hist.copyTo(dstHistImg);
	}
	else {
		cout << "ERROR! drawHist_Color()  :  Can't draw Histgram because of its channel." << endl;
	}
	cout << endl;
}

void DrawHist(Mat& targetImg, Mat& dstHistImg, int MaxCounter) {
	dstHistImg = Mat::zeros(targetImg.size(), CV_8U);

	if (targetImg.channels() == 3) {
		Mat channels[3];
		int cha_index, ha_index;
		uchar Gray;
		for (int channel = 0; channel < 3; channel++) {
			channels[channel] = Mat(Size(targetImg.cols, targetImg.rows), CV_8UC1);
			for (int i = 0; i < targetImg.rows; i++) {
				for (int j = 0; j < targetImg.cols; j++) {
					cha_index = i * targetImg.cols * 3 + j * 3 + channel;
					ha_index = i * targetImg.cols + j;
					Gray = (uchar)targetImg.data[cha_index];
					channels[channel].data[ha_index] = Gray;
				}
			}
		}

		/* 変数宣言 */
		Mat R, G, B;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* 画素数を数える */
		calcHist(&channels[0], 1, 0, Mat(), B, 1, &hist_size, &hist_range);
		calcHist(&channels[1], 1, 0, Mat(), G, 1, &hist_size, &hist_range);
		calcHist(&channels[2], 1, 0, Mat(), R, 1, &hist_size, &hist_range);

		/* 確認（ヒストグラム高さ固定のため）*/
		int MAX_COUNT = 0;
		double Min_count[3], Max_count[3];
		for (int ch = 0; ch < 3; ch++) {
			if (ch == 0) { minMaxLoc(B, &Min_count[ch], &Max_count[ch]); }
			else if (ch == 1) { minMaxLoc(G, &Min_count[ch], &Max_count[ch]); }
			else if (ch == 2) { minMaxLoc(R, &Min_count[ch], &Max_count[ch]); }
			if (Max_count[ch] > MAX_COUNT) {
				MAX_COUNT = (int)Max_count[ch];
			}
		}
		if (MAX_COUNT > MaxCounter) { cout << "WARNING! DrawHist() : MaxCounter is too small." << endl; }
		MAX_COUNT = MaxCounter;

		/* ヒストグラム生成用の画像を作成 */
		Image_hist = Mat(Size(276, 320), CV_8UC3, Scalar(255, 255, 255));

		/* 背景を描画（見やすくするためにヒストグラム部分の背景をグレーにする） */
		for (int i = 0; i < 3; i++) {
			rectangle(Image_hist, Point(10, 10 + 100 * i), Point(265, 100 + 100 * i), Scalar(230, 230, 230), -1);
		}

		for (int i = 0; i < 256; i++) {
			if (R.at<float>(i) <= MAX_COUNT * 1.15) { line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(R.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 255), 1, 8, 0); }
			else { line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)(1.15 * 80)), Scalar(0, 0, 255), 1, 8, 0); }
			if (G.at<float>(i) <= MAX_COUNT * 1.15) { line(Image_hist, Point(10 + i, 200), Point(10 + i, 200 - (int)((float)(G.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 255, 0), 1, 8, 0); }
			else { line(Image_hist, Point(10 + i, 200), Point(10 + i, 200 - (int)(1.15 * 80)), Scalar(0, 255, 0), 1, 8, 0); }
			if (B.at<float>(i) <= MAX_COUNT * 1.15) { line(Image_hist, Point(10 + i, 300), Point(10 + i, 300 - (int)((float)(B.at<float>(i) / MAX_COUNT) * 80)), Scalar(255, 0, 0), 1, 8, 0); }
			else { line(Image_hist, Point(10 + i, 300), Point(10 + i, 300 - (int)(1.15 * 80)), Scalar(255, 0, 0), 1, 8, 0); }

			if (i % 10 == 0) {		// 横軸10ずつラインを引く
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// 横軸50ずつ濃いラインを引く
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* ヒストグラム情報表示 */
		cout << "--- ヒストグラム情報 -------------------------" << endl;
		cout << " RGB画像" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "----------------------------------------------" << endl;

		/* ヒストグラム画像の出力 */
		Image_hist.copyTo(dstHistImg);
	}
	else if (targetImg.channels() == 1) {
		Mat channels;
		int ha_index;
		uchar Gray;
		channels = Mat(Size(targetImg.cols, targetImg.rows), CV_8UC1);
		for (int i = 0; i < targetImg.rows; i++) {
			for (int j = 0; j < targetImg.cols; j++) {
				ha_index = i * targetImg.cols + j;
				Gray = (uchar)targetImg.data[ha_index];
				channels.data[ha_index] = Gray;
			}
		}

		/* 変数宣言 */
		Mat GRAY;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* 画素数を数える */
		calcHist(&channels, 1, 0, Mat(), GRAY, 1, &hist_size, &hist_range);

		/* 確認（ヒストグラム高さ固定のため）*/
		int MAX_COUNT = 0;
		double Min_count, Max_count;
		minMaxLoc(GRAY, &Min_count, &Max_count);
		if (Max_count > MAX_COUNT) {
			MAX_COUNT = (int)Max_count;
		}
		if (MAX_COUNT > MaxCounter) { cout << "WARNING! DrawHist() : MaxCounter is too small." << endl; }
		MAX_COUNT = MaxCounter;

		/* ヒストグラム生成用の画像を作成 */
		Image_hist = Mat(Size(276, 120), CV_8UC3, Scalar(255, 255, 255));

		/* 背景を描画（見やすくするためにヒストグラム部分の背景をグレーにする） */
		rectangle(Image_hist, Point(10, 10), Point(265, 100), Scalar(230, 230, 230), -1);

		for (int i = 0; i < 256; i++) {
			if (GRAY.at<float>(i) <= MAX_COUNT * 1.15) { line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(GRAY.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 0), 1, 8, 0); }
			else { line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)(1.15 * 80)), Scalar(0, 0, 0), 1, 8, 0); }

			if (i % 10 == 0) {		// 横軸10ずつラインを引く
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// 横軸50ずつ濃いラインを引く
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* ヒストグラム情報表示 */
		cout << "--- ヒストグラム情報 ------------------------------" << endl;
		cout << " GrayScale画像" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "---------------------------------------------------" << endl;

		/* ヒストグラム画像の出力 */
		Image_hist.copyTo(dstHistImg);
	}
	else {
		cout << "ERROR! drawHist_Color()  :  Can't draw Histgram because of its channel." << endl;
	}
	cout << endl;
}

#endif
