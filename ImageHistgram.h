#ifndef __INCLUDED_H_ImageHistgram__
#define __INCLUDED_H_ImageHistgram__

#include "main.h"


/* ���o�͉摜 */
string win_dst_hist = "dst_hist";	// �o�͉摜�E�B���h�E
Mat Image_hist;		// �q�X�g�O�����摜

/* �֐��錾 */
void DrawHist(Mat& targetImg);					// �q�X�g�O�����v�Z&�`��
void DrawHist(Mat& targetImg, Mat& dstHistImg);	// �q�X�g�O�����v�Z


/* �֐� */
// �q�X�g�O�����v�Z&�`��
void DrawHist(Mat& targetImg) {
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

		/* �ϐ��錾 */
		Mat R, G, B;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* ��f���𐔂��� */
		calcHist(&channels[0], 1, 0, Mat(), B, 1, &hist_size, &hist_range);
		calcHist(&channels[1], 1, 0, Mat(), G, 1, &hist_size, &hist_range);
		calcHist(&channels[2], 1, 0, Mat(), R, 1, &hist_size, &hist_range);

		/* �m�F�i�q�X�g�O���������Œ�̂��߁j*/
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

		/* �q�X�g�O���������p�̉摜���쐬 */
		Image_hist = Mat(Size(276, 320), CV_8UC3, Scalar(255, 255, 255));

		/* �w�i��`��i���₷�����邽�߂Ƀq�X�g�O���������̔w�i���O���[�ɂ���j */
		for (int i = 0; i < 3; i++) {
			rectangle(Image_hist, Point(10, 10 + 100 * i), Point(265, 100 + 100 * i), Scalar(230, 230, 230), -1);
		}

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(R.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 255), 1, 8, 0);
			line(Image_hist, Point(10 + i, 200), Point(10 + i, 200 - (int)((float)(G.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 255, 0), 1, 8, 0);
			line(Image_hist, Point(10 + i, 300), Point(10 + i, 300 - (int)((float)(B.at<float>(i) / MAX_COUNT) * 80)), Scalar(255, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// ����10�����C��������
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// ����50���Z�����C��������
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* �q�X�g�O�������\�� */
		cout << "--- �q�X�g�O������� -------------------------" << endl;
		cout << " RGB�摜" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "----------------------------------------------" << endl;

		/* �q�X�g�O�����摜�̕\�� & �ۑ� */
		string file_dst_hist = "dst_hist.jpg";		// �o�͉摜�̃t�@�C����
		namedWindow(win_dst_hist, WINDOW_AUTOSIZE);	// �E�B���h�E����
		imshow(win_dst_hist, Image_hist);				// �o�͉摜��\��
		imwrite(file_dst_hist, Image_hist);			// �o�͉摜�̕ۑ�

		waitKey(0);			// �L�[���͑҂�
		destroyWindow(win_dst_hist);
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

		/* �ϐ��錾 */
		Mat GRAY;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* ��f���𐔂��� */
		calcHist(&channels, 1, 0, Mat(), GRAY, 1, &hist_size, &hist_range);

		/* �m�F�i�q�X�g�O���������Œ�̂��߁j*/
		int MAX_COUNT = 0;
		double Min_count, Max_count;
		minMaxLoc(GRAY, &Min_count, &Max_count);
		if (Max_count > MAX_COUNT) {
			MAX_COUNT = (int)Max_count;
		}
		//MAX_COUNT = 80000;

		/* �q�X�g�O���������p�̉摜���쐬 */
		Image_hist = Mat(Size(276, 120), CV_8UC3, Scalar(255, 255, 255));

		/* �w�i��`��i���₷�����邽�߂Ƀq�X�g�O���������̔w�i���O���[�ɂ���j */
		rectangle(Image_hist, Point(10, 10), Point(265, 100), Scalar(230, 230, 230), -1);

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(GRAY.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// ����10�����C��������
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// ����50���Z�����C��������
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* �q�X�g�O�������\�� */
		cout << "--- �q�X�g�O������� ------------------------------" << endl;
		cout << " GrayScale�摜" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "---------------------------------------------------" << endl;

		/* �q�X�g�O�����摜�̕\�� & �ۑ� */
		string file_dst_hist = "dst_hist.jpg";		// �o�͉摜�̃t�@�C����
		namedWindow(win_dst_hist, WINDOW_AUTOSIZE);	// �E�B���h�E����
		imshow(win_dst_hist, Image_hist);				// �o�͉摜��\��
		imwrite(file_dst_hist, Image_hist);			// �o�͉摜�̕ۑ�

		waitKey(0);			// �L�[���͑҂�
		destroyWindow(win_dst_hist);
	}
	else {
		cout << "ERROR! drawHist_Color()  :  Can't draw Histgram because of its channel." << endl;
	}
	cout << endl;
}

// �q�X�g�O�����v�Z
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

		/* �ϐ��錾 */
		Mat R, G, B;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* ��f���𐔂��� */
		calcHist(&channels[0], 1, 0, Mat(), B, 1, &hist_size, &hist_range);
		calcHist(&channels[1], 1, 0, Mat(), G, 1, &hist_size, &hist_range);
		calcHist(&channels[2], 1, 0, Mat(), R, 1, &hist_size, &hist_range);

		/* �m�F�i�q�X�g�O���������Œ�̂��߁j*/
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

		/* �q�X�g�O���������p�̉摜���쐬 */
		Image_hist = Mat(Size(276, 320), CV_8UC3, Scalar(255, 255, 255));

		/* �w�i��`��i���₷�����邽�߂Ƀq�X�g�O���������̔w�i���O���[�ɂ���j */
		for (int i = 0; i < 3; i++) {
			rectangle(Image_hist, Point(10, 10 + 100 * i), Point(265, 100 + 100 * i), Scalar(230, 230, 230), -1);
		}

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(R.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 255), 1, 8, 0);
			line(Image_hist, Point(10 + i, 200), Point(10 + i, 200 - (int)((float)(G.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 255, 0), 1, 8, 0);
			line(Image_hist, Point(10 + i, 300), Point(10 + i, 300 - (int)((float)(B.at<float>(i) / MAX_COUNT) * 80)), Scalar(255, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// ����10�����C��������
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
					Scalar(170, 170, 170), 1, 8, 0);
				line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// ����50���Z�����C��������
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 200), Point(10 + i, 110),
						Scalar(50, 50, 50), 1, 8, 0);
					line(Image_hist, Point(10 + i, 300), Point(10 + i, 210),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* �q�X�g�O�������\�� */
		cout << "--- �q�X�g�O������� -------------------------" << endl;
		cout << " RGB�摜" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "----------------------------------------------" << endl;

		/* �q�X�g�O�����摜�̏o�� */
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

		/* �ϐ��錾 */
		Mat GRAY;
		int hist_size = 256;
		float range[] = { 0, 256 };
		const float* hist_range = range;

		/* ��f���𐔂��� */
		calcHist(&channels, 1, 0, Mat(), GRAY, 1, &hist_size, &hist_range);

		/* �m�F�i�q�X�g�O���������Œ�̂��߁j*/
		int MAX_COUNT = 0;
		double Min_count, Max_count;
		minMaxLoc(GRAY, &Min_count, &Max_count);
		if (Max_count > MAX_COUNT) {
			MAX_COUNT = (int)Max_count;
		}
		//MAX_COUNT = 80000;

		/* �q�X�g�O���������p�̉摜���쐬 */
		Image_hist = Mat(Size(276, 120), CV_8UC3, Scalar(255, 255, 255));

		/* �w�i��`��i���₷�����邽�߂Ƀq�X�g�O���������̔w�i���O���[�ɂ���j */
		rectangle(Image_hist, Point(10, 10), Point(265, 100), Scalar(230, 230, 230), -1);

		for (int i = 0; i < 256; i++) {
			line(Image_hist, Point(10 + i, 100), Point(10 + i, 100 - (int)((float)(GRAY.at<float>(i) / MAX_COUNT) * 80)), Scalar(0, 0, 0), 1, 8, 0);

			if (i % 10 == 0) {		// ����10�����C��������
				line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
					Scalar(170, 170, 170), 1, 8, 0);

				if (i % 50 == 0) {	// ����50���Z�����C��������
					line(Image_hist, Point(10 + i, 100), Point(10 + i, 10),
						Scalar(50, 50, 50), 1, 8, 0);
				}
			}
		}

		/* �q�X�g�O�������\�� */
		cout << "--- �q�X�g�O������� ------------------------------" << endl;
		cout << " GrayScale�摜" << endl;
		cout << " MAX_COUNT : " << MAX_COUNT << endl;
		cout << "---------------------------------------------------" << endl;

		/* �q�X�g�O�����摜�̏o�� */
		Image_hist.copyTo(dstHistImg);
	}
	else {
		cout << "ERROR! drawHist_Color()  :  Can't draw Histgram because of its channel." << endl;
	}
	cout << endl;
}

#endif