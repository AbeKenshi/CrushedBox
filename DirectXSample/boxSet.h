#ifndef _BOXSET_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�A
#define _BOXSET_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include "box.h"

// �����������������Z�b�g��\���N���X
class BoxSet
{
private:
	std::vector<Box> boxes;
public:
	// �R���X�g���N�^
	BoxSet(Box& box1, Box& box2) {
		boxes.push_back(box1);
		boxes.push_back(box2);
	}
	BoxSet(Box& box) {
		boxes.push_back(box);
	}

	// �{�b�N�X�I�u�W�F�N�g��Ԃ�
	Box& getBox(int index) { return boxes.at(index); }

	// �{�b�N�X�Z�b�g�Ɋ܂܂��{�b�N�X�̐���Ԃ�
	int getBoxSize() {
		return boxes.size();
	}

	// �Z�b�g�Ƀ{�b�N�X��ǉ�����
	void add(Box& box) {
		boxes.push_back(box);
	}
};

#endif