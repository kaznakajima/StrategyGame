#include "CharacterManager.h"
#include "AIManager.h"

// ��������L�����N�^�[��
const int PlayerNum = 4;

CharacterManager::~CharacterManager() {
	//Finalize();
}

// ������
void CharacterManager::Initialize()
{
	FileManager::Instance()->GetFileHandle(HP_BAR);
	FileManager::Instance()->GetFileHandle(HP_BARBOX);
	FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL);

	// �L�����N�^�[�̒ǉ�
	for (size_t num = 0; num < PlayerNum; num++) {
		_character.push_back(make_shared<Character>());
	}

	// �L�����N�^�[�̏�����
	_character[0]->Character_Initialize(CHARACTER_DATA_1, "Player", 240, 240);
	_character[1]->Character_Initialize(CHARACTER_DATA_3, "Enemy", 144, 240);
	_character[2]->Character_Initialize(CHARACTER_DATA_2, "Player", 96, 384);
	_character[3]->Character_Initialize(CHARACTER_DATA_4, "Enemy", 336, 96);

	// �GAI�̏�����
	for (size_t num = 0; num < _character.size(); ++num) {
		AIManager::Instance()->CharacterCount(_character[num]);
	}

	StartTurn();
}

// �L�����N�^�[�X�V
void CharacterManager::Update(int x, int y)
{
	// �U���͈͌���
	GetAttackArea(x, y);

	for (size_t i = 0; i < _character.size(); i++) {
		// �A�j���[�V����
		_character[i]->CharacterAnim();
	}

	// �L�����N�^�[�ړ�
	if (isSelect == false) CharacterMove(x, y);

	// �U��
	if (attack) Attack();

	// �U�����̃f�[�^�\��
	if (_myCharacter != nullptr && _eCharacter != nullptr) {
		// �U�������0.5�b��������U���I��
		if (TimeCount::Instance()->GetTimer(500.0f) >= 500.0f) {
			_myCharacter.reset();
			_eCharacter.reset();
			moveableUnit--;
			for (size_t num = 0; num < _character.size(); num++) {
				// ���S�������j�b�g�����O����
				if (_character[num]->myStatus->isDeath) {
					_character[num].reset();
					_character.erase(_character.begin() + num);
					AIManager::Instance()->Initialize();
					// �GAI�̏�����
					for (size_t num = 0; num < _character.size(); ++num) {
						AIManager::Instance()->CharacterCount(_character[num]);
					}
				}
			}
			if (moveableUnit <= 0) { StartTurn(); return; }
			if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
			return;
		}
		DrawAttackParam(_myCharacter, _eCharacter);
	}
}

// �^�[���J�n
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->TurnStart();

		// ���S�������j�b�g�����O����
		if (_character[num]->myStatus->isDeath) {
			_character[num].reset();
			_character.erase(_character.begin() + num);
			AIManager::Instance()->Initialize();
			// �GAI�̏�����
			for (size_t num = 0; num < _character.size(); ++num) {
				AIManager::Instance()->CharacterCount(_character[num]);
			}
		}
		else {
			// �ړ��\�ȃ��j�b�g�̃J�E���g
		    // �v���C���[�����j�b�g�̌v�Z
			if (playerTurn && _character[num]->myStatus->myTeam == "Player") moveableUnit++;
			// �G�����j�b�g�̌v�Z
			else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
		}
	}

	AudioManager::Instance()->playSE(SE_TURNSTART);
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

	for (size_t num = 0; num < _character.size(); num++) {
		// �v���C���[�^�[��
		if (playerTurn && _character[num]->myStatus->myTeam == "Player") {
			// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
			if (_character[num]->myStatus->PosX == x && _character[num]->myStatus->PosY == y) {
				if (_character[num]->myStatus->canMove) {
					_character[num]->myStatus->isSelect = true;
					_character[num]->myStatus->AnimHandle = 4.0f;
					isSelect = true;
					attack = false;
				}
			}
		}
		// �G�^�[��
		else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") {
			// �J�[�\���������Ă��郆�j�b�g�̂ݕ\��
			if (_character[num]->myStatus->PosX == x && _character[num]->myStatus->PosY == y) {
				if (_character[num]->myStatus->canMove) {
					_character[num]->myStatus->isSelect = true;
					_character[num]->myStatus->AnimHandle = 4.0f;
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
	for (size_t num = 0; num < _character.size(); num++) {
		// �ړ����H���L�^���ړ��͈͂ƍU���͈͂̕`��
		if (_character[num]->myStatus->isSelect) {
			_character[num]->MoveAreaClear(_character);
			_character[num]->OldPosX.push_back(_character[num]->myStatus->PosX);
			_character[num]->OldPosY.push_back(_character[num]->myStatus->PosY);
			_character[num]->MoveRange(_character[num]->myStatus->PosX, _character[num]->myStatus->PosY, _character[num]->myStatus->myParam.MOVERANGE);
			_character[num]->AttackRange();
			return;
		}
	}
}

void CharacterManager::CharacterMove(int x, int y) 
{
	// �ړ������������ǂ���
	bool moveEnd = false;

	// �L�����N�^�[�̈ړ�
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->isSelect) {
			isMove = _character[num]->CharacterMove(x, y);

			// �ړ����������A�U�����Ȃ��Ȃ�s���I��
			if (isMove == false && _character[num]->myStatus->canMove == false
				&& _character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
			}
		}
	}

	// �s���\�ȃ��j�b�g�����Ȃ��Ȃ�^�[���I��
	if (moveEnd && playerTurn == false && moveableUnit != 0) AIManager::Instance()->Play();
	if (moveableUnit <= 0) StartTurn();
}

// �ړ��l�̎擾
void CharacterManager::GetMoveCount(int x, int y) {
	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->OldPosX.push_back(x);
		_character[num]->OldPosY.push_back(y);
		_character[num]->moveCount++;
	}
}

void CharacterManager::GetMoveArrow(int x, int y)
{
	// �ړ����H��`��
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->isSelect) {
			_character[num]->DrawMoveArrow(x, y, 5);

			// ���j�b�g�̈ʒu�ɖ߂����珇�H���N���A
			if (_character[num]->myStatus->PosX == x && _character[num]->myStatus->PosY == y) {
				_character[num]->OldPosX.clear();
				_character[num]->OldPosY.clear();
				_character[num]->moveCount = 0;
			}
		}
	}
}

// �U���͈͕\��
void CharacterManager::GetAttackArea(int x, int y)
{
	if (attackCount != 0) return;

	// �U���\�ȃ��j�b�g�̎擾
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->canAttack) {
			_myCharacter = _character[num];
			_myCharacter->AttackableDraw();
			attack = true;
		}
	}

	if (playerTurn == false) return;

	// �U���\�Ȉʒu�̃��j�b�g�Ƃ̍U�������ۂ̏ڍ׏��̕\��
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != nullptr && _myCharacter->myStatus->canAttack) {
			if (_character[num]->myStatus->PosX == x && _character[num]->myStatus->PosY == y
				&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
				if (_myCharacter->myStatus->myTeam != _character[num]->myStatus->myTeam) _myCharacter->GetAttackDetail(_character[num]);
			}
		}
	}
}

// �U���̑Ώۂ�I��
void CharacterManager::ChoiseAttack(int x, int y)
{
	// �I�������ʒu�ɓG��������U���Ώۂ̃��t�@�����X���쐬
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != _character[num] && _character[num]->myStatus->PosX == x && _character[num]->myStatus->PosY == y) {
			_eCharacter = _character[num];
			_myCharacter->myStatus->isAttack = true;
		}

		// �U���\�ȃ��j�b�g�����Ȃ�������I��
		//if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// �I�������ʒu�ɓG�����Ȃ��Ȃ�I��
	if (_eCharacter == nullptr) {
		attack = false;
		moveableUnit--;
		if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
		if (moveableUnit == 0) StartTurn();
	}
}

// �U���̃A�j���[�V����
void CharacterManager::Attack()
{
	if (_myCharacter == nullptr || _eCharacter == nullptr) return;

	// 1��ڂ̍U��
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount < 1) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 1);
		if (_eCharacter->myStatus->isAttack && _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 2);

		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I����Ă���Ȃ�U���̉񐔂��L�^
	if(attack == false) attackCount = 3;

	// 2��ڂ̍U��
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount > 2) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 3);
		if (_eCharacter->myStatus->isAttack&& _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 4);
		
		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) return;
	}

	// �A�j���[�V�������I������Ȃ�U���I��
	if (attack == false) {
		TimeCount::Instance()->SetCount();
		attackCount = 0;
		_myCharacter->myStatus->canAttack = false;
		_eCharacter->myStatus->canAttack = false;
	}
}

// �U�����̃f�[�^�\��
void CharacterManager::DrawAttackParam(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara)
{
	float drawOffset = 150;

	// �U�������j�b�g�̈ʒu�ɉ����ĕ\���ʒu��ύX
	if (attackChara->myStatus->_PosY >= STAGE1_HEIGHT / 2) {
		drawOffset = -100;
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}
	else if (attackChara->myStatus->_PosY < STAGE1_HEIGHT / 2) {
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}

	// ���(�̗́A���O)�̕\���ʒu�̒�`
	float A_drawPosX = 390.0f, A_drawPosY = 260 + drawOffset;
	float D_drawPosX = 190.0f, D_drawPosY = 260 + drawOffset;

	// �U�����̏��̕`��
	DrawFormatString((int)A_drawPosX, (int)A_drawPosY - 50, GetColor(0, 0, 0), attackChara->myStatus->NAME.c_str());
	DrawFormatString((int)A_drawPosX - 30, (int)A_drawPosY, GetColor(0, 0, 0), "%d", attackChara->myStatus->myParam.HP);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + 100, A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + (100 * ((float)attackChara->myStatus->myParam.HP / (float)attackChara->myStatus->myParam.MaxHP)), A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);

	// �h�䑤�̏��̕`��
	DrawFormatString((int)D_drawPosX, (int)D_drawPosY - 50, GetColor(0, 0, 0), defenceChara->myStatus->NAME.c_str());
	DrawFormatString((int)D_drawPosX - 30, (int)D_drawPosY, GetColor(0, 0, 0), "%d", defenceChara->myStatus->myParam.HP);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + 100, D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + (100 * ((float)defenceChara->myStatus->myParam.HP / (float)defenceChara->myStatus->myParam.MaxHP)), D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);
}

// �J�����Ƃ̃I�t�Z�b�g�̌v�Z
void CharacterManager::SetCameraOffset(int dir, bool horizontal)
{
	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->SetCameraOffset(dir, horizontal);
	}
}

// ���͂����n�_�̃`�F�b�N
void CharacterManager::KeyCheck(int x, int y)
{
	for (size_t num = 0; num < _character.size(); num++) {
		// �J�[�\���������Ă��Ȃ���ΑI����Ԃ�����
		if (_character[num]->myStatus->PosX != x || _character[num]->myStatus->PosY != y) {
			_character[num]->OldPosX.clear();
			_character[num]->OldPosY.clear();
			_character[num]->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::Finalize() {
	if (_character.empty() == false) _character.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
	if (_eCharacter != nullptr) _eCharacter.reset();
}