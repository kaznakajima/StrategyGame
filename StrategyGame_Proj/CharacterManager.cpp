#include "CharacterManager.h"
#include "AIManager.h"

// ��������L�����N�^�[��
const int PlayerNum = 4;

AIManager* AIMgr;

// �L�����N�^�[�̏��
vector<Character::STATUS> statusList;
Character::STATUS status[PlayerNum];

// ������
void CharacterManager::Initialize()
{
	AIMgr = AIManager::Instance();

	// �L�����N�^�[�̒ǉ�
	for (size_t num = 0; num < PlayerNum; num++) {
		//statusList.push_back(status[num]);
		character.push_back(new Character());
	}

	// �L�����N�^�[�̏�����
	character[0]->Character_Initialize(CHARACTER_DATA_1, "�G�C���[�N", "Player", 240, 240);
	character[1]->Character_Initialize(CHARACTER_DATA_2, "���@���^�[", "Enemy", 144, 240);
	character[2]->Character_Initialize(CHARACTER_DATA_3, "�[�g", "Player", 96, 384);
	character[3]->Character_Initialize(CHARACTER_DATA_1, "�N�[�K�[", "Enemy", 336, 96);

	// �GAI�̏�����
	for (Character* _character : character) {
		AIMgr->CharacterCount(_character);
	}

	StartTurn();
}

// �L�����N�^�[�X�V
void CharacterManager::Update(int x, int y)
{
	// �U���͈͌���
	GetAttackArea(x, y);

	if (myCharacter != nullptr) {
		DrawFormatString(0, 96, GetColor(0, 0, 0), myCharacter->myStatus->myParam.NAME.c_str());
	}

	for (size_t i = 0; i < character.size(); i++) {
		// �A�j���[�V����
		character[i]->CharacterAnim();
	}

	DrawFormatString(0, 0, GetColor(0, 0, 0), "[%d]", character.size());

	if (isSelect == false) CharacterMove(x, y);

	if (attack) Attack();
}

// �^�[���J�n
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < character.size(); num++) {
		character[num]->TurnStart();

		// ���S�������j�b�g�����O����
		if (character[num]->myStatus->isDeath) {
			character.erase(character.begin() + num);
			AIMgr->Initialize();
			// �GAI�̏�����
			for (Character* _character : character) {
				AIMgr->CharacterCount(_character);
			}
		}
		else {
			// �ړ��\�ȃ��j�b�g�̃J�E���g
		    // �v���C���[�����j�b�g�̌v�Z
			if (playerTurn && character[num]->myStatus->myTeam == "Player") moveableUnit++;
			// �G�����j�b�g�̌v�Z
			else if (playerTurn == false && character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
		}
	}

	turnAnim = true;
}

// �`�悷�邩�`�F�b�N
void CharacterManager::DrawCheck(int x, int y) 
{
	// �U�����̓��^�[��
	if (attack == true) {
		ChoiseAttack(x, y);
		return;
	}

	for (size_t num = 0; num < character.size(); num++) {
		if (playerTurn && character[num]->myStatus->myTeam == "Player") {
			// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				if (character[num]->myStatus->canMove) {
					character[num]->myStatus->isSelect = true;
					character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		else if (playerTurn == false && character[num]->myStatus->myTeam == "Enemy") {
			// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				if (character[num]->myStatus->canMove) {
					character[num]->myStatus->isSelect = true;
					character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		
	}
}

// �`�揈��
void CharacterManager::Draw()
{
	for (size_t num = 0; num < character.size(); num++) {
		// �ړ����H���L�^���ړ��͈͂ƍU���͈͂̕`��
		if (character[num]->myStatus->isSelect) {
			character[num]->MoveAreaClear(character);
			character[num]->OldPosX.push_back(character[num]->myStatus->PosX);
			character[num]->OldPosY.push_back(character[num]->myStatus->PosY);
			character[num]->MoveRange(character[num]->myStatus->PosX, character[num]->myStatus->PosY, character[num]->myStatus->myParam.MOVERANGE);
			character[num]->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	bool moveEnd = false;

	// �L�����N�^�[�̈ړ�
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->isSelect) {
			isMove = character[num]->CharacterMove(x, y);

			// �ړ����������A�U�����Ȃ��Ȃ�s���I��
			if (isMove == false && character[num]->myStatus->canMove == false
				&& character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
			}
		}
	}

	// �s���\�ȃ��j�b�g�����Ȃ��Ȃ�^�[���I��
	if (moveEnd && playerTurn == false && moveableUnit != 0) AIMgr->Play();
	if (moveableUnit <= 0) StartTurn();
}

// �ړ��l�̎擾
void CharacterManager::GetMoveCount(int x, int y) {
	for (size_t num = 0; num < character.size(); num++) {
		character[num]->OldPosX.push_back(x);
		character[num]->OldPosY.push_back(y);
		character[num]->moveCount++;
	}
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	// �ړ����H��`��
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->isSelect) {
			character[num]->DrawMoveArrow(x, y, 5);

			// ���j�b�g�̈ʒu�ɖ߂����珇�H���N���A
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
				character[num]->OldPosX.clear();
				character[num]->OldPosY.clear();
				character[num]->moveCount = 0;
			}
		}
	}
}

// �U���͈͕\��
void CharacterManager::GetAttackArea(int x, int y)
{
	// �U���\�ȃ��j�b�g�̎擾
	for (size_t num = 0; num < character.size(); num++) {
		if (character[num]->myStatus->canAttack) {
			myCharacter = character[num];
			myCharacter->AttackableDraw();
			attack = true;
		}
	}

	// �U���\�Ȉʒu�̃��j�b�g�Ƃ̍U�������ۂ̏ڍ׏��̕\��
	for (size_t num = 0; num < character.size(); num++) {
		if (myCharacter != nullptr && myCharacter->myStatus->canAttack) {
			if (character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y
				&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
				if (myCharacter->myStatus->myTeam != character[num]->myStatus->myTeam) myCharacter->GetAttackDetail(character[num]);
			}
		}
	}
}

// �U���̑Ώۂ�I��
void CharacterManager::ChoiseAttack(int x, int y)
{
	// �I�������ʒu�ɓG��������U���Ώۂ̃��t�@�����X���쐬
	for (size_t num = 0; num < character.size(); num++) {
		if (myCharacter != character[num] && character[num]->myStatus->PosX == x && character[num]->myStatus->PosY == y) {
			eCharacter = character[num];
			myCharacter->myStatus->isAttack = true;
		}

		// �U���\�ȃ��j�b�g�����Ȃ�������I��
		if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// �I�������ʒu�ɓG�����Ȃ��Ȃ�I��
	if (eCharacter == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit != 0 && playerTurn == false) AIMgr->Play();
		if (moveableUnit == 0) StartTurn();
	}
}

// �U���̃A�j���[�V����
void CharacterManager::Attack()
{
	// 1��ڂ̍U��
	if (myCharacter != nullptr && eCharacter != nullptr && attackCount < 2) {
		if (myCharacter->myStatus->isAttack) attack = myCharacter->AttackAnimation(eCharacter, 1);
		if (eCharacter->myStatus->isAttack) attack = eCharacter->AttackAnimation(myCharacter, 2);

		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I����Ă���Ȃ�U���̉񐔂��L�^
	if(attack == false) attackCount = 3;

	// 2��ڂ̍U��
	if (myCharacter != nullptr && eCharacter != nullptr && attackCount > 2) {
		if (myCharacter->myStatus->isAttack) attack = myCharacter->AttackAnimation(eCharacter, 3);
		if (eCharacter->myStatus->isAttack) attack = eCharacter->AttackAnimation(myCharacter, 4);
		
		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I������Ȃ�U���I��
	if (attack == false) {
		attackCount = 0;
		myCharacter = nullptr;
		eCharacter = nullptr;
		moveableUnit--;
		for (size_t num = 0; num < character.size(); num++) {
			// ���S�������j�b�g�����O����
			if (character[num]->myStatus->isDeath) {
				character.erase(character.begin() + num);
				AIMgr->Initialize();
				// �GAI�̏�����
				for (Character* _character : character) {
					AIMgr->CharacterCount(_character);
				}
			}
		}
		if (moveableUnit != 0 && playerTurn == false) AIMgr->Play();
		if (moveableUnit <= 0) StartTurn();
	}
}

// �J�����Ƃ̃I�t�Z�b�g�̌v�Z
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (size_t num = 0; num < character.size(); num++) {
		character[num]->SetCameraOffset(dir, horizontal);
	}
}

void CharacterManager::KeyCheck(int x, int y)
{
	for (size_t i = 0; i < character.size(); i++) {
		// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
		if (character[i]->myStatus->PosX != x || character[i]->myStatus->PosY != y) {
			character[i]->OldPosX.clear();
			character[i]->OldPosY.clear();
			character[i]->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {

}