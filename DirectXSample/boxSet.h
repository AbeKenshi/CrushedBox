#ifndef _BOXSET_H	// このファイルが複数の箇所でインクルードされる場合、
#define _BOXSET_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include "box.h"

// 箱が複数個くっついたセットを表すクラス
class BoxSet
{
private:
	std::vector<Box> boxes;
public:
	// コンストラクタ
	BoxSet(Box& box1, Box& box2) {
		boxes.push_back(box1);
		boxes.push_back(box2);
	}
	BoxSet(Box& box) {
		boxes.push_back(box);
	}

	// ボックスオブジェクトを返す
	Box& getBox(int index) { return boxes.at(index); }

	// ボックスセットに含まれるボックスの数を返す
	int getBoxSize() {
		return boxes.size();
	}

	// セットにボックスを追加する
	void add(Box& box) {
		boxes.push_back(box);
	}
};

#endif