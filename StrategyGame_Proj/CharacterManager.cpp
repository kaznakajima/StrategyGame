#include "CharacterManager.h"
#include "AIManager.h"
#include "Character.h"

// ��������L�����N�^�[��
const int PlayerNum = 3;

// AIManager�C���X�^���X
AIManager* AIMgr;

// �L�����N�^�[�̏��
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// �L�����N�^�[�̃C���X�^���X
Character* character;
// �U�����郆�j�b�g
Character::STATUS* myStatus = nullptr;
// �U������郆�j�b�g
Character::STATUS* eStatus = nullptr;

// �f�X�g���N�^
CharacterManager::~CharacterManager()
{
	delete AIMgr;
	delete character;
}

// ������
void CharacterManager::Initialize()
{
	AIMgr = new AIManager();
	character = new Character();

	// �L�����N�^�[�̒ǉ�
	for (unsigned int num = 0; num < PlayerNum; num ++) {
		statusList.push_back(status[num]);
	}

	// �L�����N�^�[�̏�����
	character->Character_Initialize(&statusList[0], CHARACTER_DATA_1, "Player", 240, 240);
	character->Character_Initialize(&statusList[1], CHARACTER_DATA_2, "Enemy", 144, 336);
	character->Character_Initialize(&statusList[2], CHARACTER_DATA_3, "Player", 96, 384);

	// �GAI�̏�����
	for (Character::STATUS &status : statusList) {
		AIMgr->CharacterCount(status);
	}

	StartTurn();
}

// �L�����N�^�[�X�V
void CharacterManager::Update(int x, int y)
{
	// �U���͈͌���
	GetAttackArea(x, y);

	for (unsigned int i = 0; i < statusList.size(); i++) {
		// ���S�������j�b�g�����O����
		if (statusList[i].isDeath) {
			statusList.erase(statusList.begin() + i);
			return;
		}

		character->CharacterAnim(&statusList[i]);
	}

	if (isSelect == false) CharacterMove(x, y);

	if (attack) Attack();
}

// �^�[���J�n
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	// �ړ��\�ȃ��j�b�g�̃J�E���g
	for (Character::STATUS &status : statusList) {
		// �v���C���[�^�[��
		if (playerTurn && status.myTeam == "Player") moveableUnit++;
		// �G�^�[��
		else if (playerTurn == false && status.myTeam == "Enemy") moveableUnit++;
	}

	// �G�^�[���̊J�n
	if (playerTurn == false) AIMgr->Play();
}

// �`�悷�邩�`�F�b�N
void CharacterManager::DrawCheck(int x, int y) 
{
	// �U�����̓��^�[��
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (Character::STATUS &status : statusList) {
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (status.PosX == x && status.PosY == y) {
			if (status.canMove) {
				status.isSelect = true;
				status.AnimHandle = 4.0f;
				isSelect = true;
				attack = false;
			}
		}
	}
}

// �`�揈��
void CharacterManager::Draw()
{
	for (Character::STATUS &status : statusList) {
		character->MoveAreaClear();

		// �ړ����H���L�^���ړ��͈͂ƍU���͈͂̕`��
		if (status.isSelect) {
			character->OldPosX.push_back(status.PosX);
			character->OldPosY.push_back(status.PosY);
			character->MoveRange(&status, status.PosX, status.PosY, status.myParam.MOVERANGE);
			character->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	// �L�����N�^�[�̈ړ�
	for (Character::STATUS &status : statusList) {
		if (status.isSelect) {
			isMove = character->CharacterMove(&status, x, y);

			// �ړ����������A�U�����Ȃ��Ȃ�s���I��
			if (isMove == false && status.canMove == false && status.canAttack == false) moveableUnit--;
		}
	}

	// �s���\�ȃ��j�b�g�����Ȃ��Ȃ�^�[���I��
	if (moveableUnit <= 0) StartTurn();
}

// �ړ��l�̎擾
void CharacterManager::GetMoveCount(int x, int y) {
	character->OldPosX.push_back(x);
	character->OldPosY.push_back(y);
	character->moveCount++;
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	// �ړ����H��`��
	for (Character::STATUS &status : statusList) {
		if (status.isSelect) {
			character->DrawMoveArrow(status, x, y, 5);

			// ���j�b�g�̈ʒu�ɖ߂����珇�H���N���A
			if (status.PosX == x && status.PosY == y) {
				character->OldPosX.clear();
				character->OldPosY.clear();
				character->moveCount = 0;
			}
		}
	}
}

// �U���͈͕\��
void CharacterManager::GetAttackArea(int x, int y)
{
	// �L�����N�^�[���Ϗܗp
	Character::STATUS _status;

	// �U���\�ȃ��j�b�g�̎擾
	for (Character::STATUS &status : statusList) {
		if (status.canAttack) {
			myStatus = &status;
			_status = status;
			character->AttackableDraw(status);
			attack = true;
		}
	}

	// �U���\�ȃ��j�b�g������Ȃ�U�������ۂ̏ڍ׏���`��
	for (Character::STATUS &status : statusList) {
		if (myStatus != nullptr && myStatus != &status) {
			if (status.PosX == x && status.PosY == y && status.myTeam != myStatus->myTeam) 
				character->GetAttackDetail(_status, status);
		}
	}
}

// �U���̑Ώۂ�I��
void CharacterManager::ChoiseAttack(int x, int y)
{
	// �I�������ʒu�ɓG��������U���Ώۂ̃��t�@�����X���쐬
	for (Character::STATUS &status : statusList) {
		if (myStatus != &status) {
			if (status.PosX == x && status.PosY == y) {
				eStatus = &status;
				myStatus->isAttack = true;
			}
		}
	}

	// �U���\�ȃ��j�b�g�����Ȃ�������I��
	if(myStatus != nullptr) myStatus->canAttack = false;
	// �I�������ʒu�ɓG�����Ȃ��Ȃ�I��
	if (eStatus == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit == 0) StartTurn();
	}
}

// �U���̃A�j���[�V����
void CharacterManager::Attack()
{
	// 1��ڂ̍U��
	if (myStatus != nullptr && eStatus != nullptr && attackCount < 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 1);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 2);

		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I����Ă���Ȃ�U���̉񐔂��L�^
	if(attack == false) attackCount = 3;

	// 2��ڂ̍U��
	if (myStatus != nullptr && eStatus != nullptr && attackCount > 2) {
		if (myStatus->isAttack) attack = character->AttackAnimation(myStatus, eStatus, 3);
		if (eStatus->isAttack) attack = character->AttackAnimation(eStatus, myStatus, 4);
		
		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I������Ȃ�U���I��
	if (attack == false) {
		attackCount = 0;
		myStatus = nullptr;
		eStatus = nullptr;
		moveableUnit--;
		if (moveableUnit <= 0) StartTurn();
	}
}

// �J�����Ƃ̃I�t�Z�b�g�̌v�Z
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (Character::STATUS &status : statusList) {
		character->SetCameraOffset(&status, dir, horizontal);
	}
}

void CharacterManager::KeyCheck(int x, int y)
{
	for (unsigned int i = 0; i < statusList.size(); i++) {
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (statusList[i].PosX != x || statusList[i].PosY != y) {
			character->OldPosX.clear();
			character->OldPosY.clear();
			character->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {

}