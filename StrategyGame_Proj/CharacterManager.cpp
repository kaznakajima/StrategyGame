#include "CharacterManager.h"
#include "AIManager.h"

CharacterManager::~CharacterManager() {}

// ������
void CharacterManager::Initialize()
{
	SetCharacterData();

	FileManager::Instance()->GetFileHandle(HP_BAR);
	FileManager::Instance()->GetFileHandle(HP_BARBOX);
	FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL);

	int allCharacter = StageCreate::Instance()->playerCount + StageCreate::Instance()->enemyCount;
	// �L�����N�^�[�̒ǉ�
	for (size_t num = 0; num < allCharacter; num++) {
		_character.push_back(make_shared<Character>());
	}

	for (int num = 0; num < StageCreate::Instance()->playerCount; num++) {
		_character[num]->Character_Initialize(playerDataPass[num], "Player", StageCreate::Instance()->p_InitPos[num * 2], StageCreate::Instance()->p_InitPos[num * 2 + 1]);
	}
	for (int num = 0; num < StageCreate::Instance()->enemyCount; num++) {
		_character[StageCreate::Instance()->playerCount + num]->Character_Initialize(enemyDataPass[num], "Enemy", StageCreate::Instance()->e_InitPos[num * 2], StageCreate::Instance()->e_InitPos[num * 2 + 1]);
	}

	// �GAI�̏�����
	for (size_t num = 0; num < _character.size(); ++num) {
		CharacterCount(_character[num]);
	}
	AIManager::Instance()->Initialize();

	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			shared_ptr<Renderer> ren = make_shared<Renderer>(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), 2, 0);
			ren->SetPosition(x * CHIP_SIZE, y * CHIP_SIZE);
			ren->SetVisible(false);
			DrawManager::Instance()->AddDrawList(ren);
			ren = make_shared<Renderer>(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), 2, 0);
			ren->SetPosition(x * CHIP_SIZE, y * CHIP_SIZE);
			ren->SetVisible(false);
			DrawManager::Instance()->AddDrawList(ren);
		}
	}

	playerTurn = false;
	isGame = true;
	StartTurn();
}

// ���j�b�g��I����Ԃɂ���
void CharacterManager::CanSelectCharacter(shared_ptr<Character> const & character, int x, int y)
{
	// �G�^�[���͏��O
	if (playerTurn == false && character->myStatus->myTeam == "Player") return;

	// �J�[�\���ʒu�Ɠ������j�b�g��I����Ԃɂ���
	if (character->myStatus->xPos == x && character->myStatus->yPos == y) {
		if (character->myStatus->canMove) {
			character->myStatus->isSelect = true;
			character->myStatus->AnimHandle = 4.0f;
			isSelect = true;
			attack = false;
			Draw();
		}
	}
}

// �f�[�^�p�X�̐ݒ�
void CharacterManager::SetCharacterData()
{
	// �v���C���[�p�X�̐ݒ�
	playerDataPass = vector<string>(StageCreate::Instance()->playerCount, "");
	for (int pass = 0; pass < playerDataPass.size(); ++pass) {
		switch (pass) {
		// ���[�h
		case 0:
			playerDataPass[pass] = UNIT_LOAD;
			break;
		// �T�|�[�g
		case 1:
			playerDataPass[pass] = UNIT_SUPPORT;
			break;
		}
	}

	// �G�̃p�X�̐ݒ�
	enemyDataPass = vector<string>(StageCreate::Instance()->enemyCount, "");
	for (int pass = 0; pass < enemyDataPass.size(); ++pass) {
		switch (pass) {
		// �G1
		case 0:
			enemyDataPass[pass] = UNIT_ENEMY1;
			break;
		// �G2
		case 1:
			enemyDataPass[pass] = UNIT_ENEMY2;
			break;
		default:
			enemyDataPass[pass] = UNIT_ENEMY1;
			break;
		}
	}
}

// ���j�b�g���X�g�̃��Z�b�g
void CharacterManager::ResetCharacterList()
{
	_playerList.clear();
	_enemyList.clear();
}

// // ���݂̓G(AI)�A�v���C���[�̃J�E���g
void CharacterManager::CharacterCount(shared_ptr<Character> const & character)
{
	if (character->myStatus->myTeam == "Player") _playerList.push_back(character);
	else if (character->myStatus->myTeam == "Enemy") _enemyList.push_back(character);
}

// �L�����N�^�[�X�V
void CharacterManager::Update(int x, int y)
{
	// �U���\���`�F�b�N
	GetAttackArea(x, y);

	for (size_t num = 0; num < _character.size(); ++num) {
		// �A�j���[�V����
		_character[num]->CharacterAnim();

		DrawAttackDetail(_character[num], x, y);
	}

	// �ڍ׏��`��
	if (checkCharacter != nullptr) checkCharacter->DrawCharacterDetail();

	// �L�����N�^�[�ړ�
	if (isSelect == false) CharacterMove(x, y);

	// �U��
	if (attack) Attack();

	// �U�����łȂ��Ȃ烊�^�[��
	if (_myCharacter == nullptr || _eCharacter == nullptr) return;

	// �U�����Ȃ�ȉ��̏��������s�����

	// �U�������0.5�b��������U���I��
	if (TimeCount::Instance()->GetTimer(500.0f) >= 500.0f) {
		_myCharacter.reset();
		_eCharacter.reset();
		moveableUnit--;
		for (size_t num = 0; num < _character.size(); num++) {
			// ���S�������j�b�g�����O����
			if (_character[num]->myStatus->isDeath) {
				// �v���C���[�w���������ꂽ��Q�[���I��
				if (_character[num]->myStatus->NAME == "Load") {
					_character[num].reset();
					_character.erase(_character.begin() + num);
					_playerList.clear(); 
					isGame = false;
					return;
				}
				_character[num].reset();
				_character.erase(_character.begin() + num);
				// ���j�b�g���X�g�̏�����
				ResetCharacterList();
				for (size_t num = 0; num < _character.size(); ++num) {
					CharacterCount(_character[num]);
				}
				AIManager::Instance()->Initialize();
			}
		}
		// �v���C���[�܂��͓G��1�̂��c���Ă��Ȃ��Ȃ�Q�[���I��
		if (_playerList.empty() || _enemyList.empty()) { isGame = false; return; }
		if (moveableUnit <= 0) { StartTurn(); return; }
		if (moveableUnit != 0 && playerTurn == false) AIManager::Instance()->Play();
		return;
	}
	DrawDamageDetail(_myCharacter, _eCharacter);
}

// �^�[���J�n
void CharacterManager::StartTurn()
{
	playerTurn = !playerTurn;
	moveableUnit = 0;

	for (size_t num = 0; num < _character.size(); num++) {
		_character[num]->TurnStart();

		// �ړ��\�ȃ��j�b�g�̃J�E���g
		// �v���C���[�����j�b�g�̌v�Z
		if (playerTurn && _character[num]->myStatus->myTeam == "Player") moveableUnit++;
		// �G�����j�b�g�̌v�Z
		else if (playerTurn == false && _character[num]->myStatus->myTeam == "Enemy") moveableUnit++;
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

	for (size_t num = 0; num < _character.size(); num++) {
		// ���j�b�g���I���\���`�F�b�N
		CanSelectCharacter(_character[num], x, y);
	}
}

// �`�揈��
void CharacterManager::Draw()
{
	for (size_t num = 0; num < _character.size(); num++) {
		// �ړ����H���L�^���ړ��͈͂ƍU���͈͂̕`��
		if (_character[num]->myStatus->isSelect) {
			_character[num]->OldPosX.push_back(_character[num]->myStatus->xPos);
			_character[num]->OldPosY.push_back(_character[num]->myStatus->yPos);
			_character[num]->MoveRange(_character[num]->myStatus->xPos, _character[num]->myStatus->yPos, _character[num]->myStatus->myParam.MOVERANGE);
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
			if (playerTurn && _character[num]->myStatus->myTeam == "Enemy") {
				_character[num]->myStatus->isSelect = false;
				_character[num]->myStatus->AnimHandle = 0.0f;
				return;
			}

			isMove = _character[num]->CharacterMove(x, y);

			// �ړ����������A�U�����Ȃ��Ȃ�s���I��
			if (isMove == false && _character[num]->myStatus->canMove == false
				&& _character[num]->myStatus->canAttack == false) {
				moveableUnit--;
				moveEnd = true;
				MoveAreaClear();
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
			if (playerTurn && _character[num]->myStatus->myTeam == "Enemy") return;

			_character[num]->DrawMoveArrow(x, y, 5);

			// ���j�b�g�̈ʒu�ɖ߂����珇�H���N���A
			if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
				_character[num]->OldPosX.clear();
				_character[num]->OldPosY.clear();
				_character[num]->moveCount = 0;
			}
		}
	}
}

void CharacterManager::MoveAreaClear()
{
	for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
		for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
			int posX = x * CHIP_SIZE, posY = y * CHIP_SIZE;
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_MOVE_AREA), posX, posY)->SetVisible(false);
			DrawManager::Instance()->GetDrawParts(FileManager::Instance()->GetFileHandle(CAN_ATTACK_AREA), posX, posY)->SetVisible(false);
			for (size_t num = 0; num < _character.size(); ++num) {
				_character[num]->ResetArea(x, y);
				StageCreate::Instance()->StageUpdate(x, y);
				if (_character[num]->myStatus->xPos == x * CHIP_SIZE
					&& _character[num]->myStatus->yPos == y * CHIP_SIZE) StageCreate::Instance()->CheckOnUnit(x, y, _character[num]->myStatus->myTeam);
			}
		}
	}
}

// �U���\�G���A�擾
void CharacterManager::GetAttackArea(int x, int y)
{
	// �U���\�ȃ��j�b�g�̎擾
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->canAttack) {
			_myCharacter = _character[num];
			_myCharacter->AttackableDraw();
			attack = true;
		}
	}
}

// �U���̏ڍ׏��\��
void CharacterManager::DrawAttackDetail(shared_ptr<Character> const &character, int x, int y)
{
	// �U���ł��郆�j�b�g�����Ȃ��A�G�^�[���A�U�����̓��^�[���A
	if (_myCharacter == nullptr || playerTurn == false || attackCount != 0) return;

	// �U���\�Ȉʒu�̃��j�b�g�Ƃ̍U�������ۂ̏ڍ׏��̕\��
	if (_myCharacter->myStatus->canAttack) {
		if (character->myStatus->xPos == x && character->myStatus->yPos == y
			&& StageCreate::Instance()->onUnit[y / CHIP_SIZE][x / CHIP_SIZE] == "Enemy") {
			if (_myCharacter->myStatus->myTeam != character->myStatus->myTeam) _myCharacter->GetAttackDetail(character);
		}
	}
}

// �U���̑Ώۂ�I��
void CharacterManager::ChoiseAttack(int x, int y)
{
	// �I�������ʒu�ɓG��������U���Ώۂ̃��t�@�����X���쐬
	for (size_t num = 0; num < _character.size(); num++) {
		if (_myCharacter != _character[num] && _character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
			if (_myCharacter->myStatus->myTeam != _character[num]->myStatus->myTeam) {
				_eCharacter = _character[num];
				_myCharacter->myStatus->isAttack = true;
			}
		}

		// �U���\�ȃ��j�b�g�����Ȃ�������I��
		//if (myCharacter == character[num]) character[num]->myStatus->canAttack = false;
	}
	// �I�������ʒu�ɓG�����Ȃ��Ȃ�I��
	if (_eCharacter == nullptr) {
		_myCharacter->myStatus->canAttack = false;
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
	if (_myCharacter != nullptr && _eCharacter != nullptr && attackCount < 2) {
		if (_myCharacter->myStatus->isAttack) attack = _myCharacter->AttackAnimation(_eCharacter, 1);
		if (_eCharacter->myStatus->isAttack && _myCharacter->myStatus->isAttack == false) attack = _eCharacter->AttackAnimation(_myCharacter, 2);

		// �A�j���[�V�������I����Ă��Ȃ��Ȃ烊�^�[��
		if (attack) { attackCount = 1; return; }
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
void CharacterManager::DrawDamageDetail(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara)
{
	float drawOffset = 150;

	// �U�������j�b�g�̈ʒu�ɉ����ĕ\���ʒu��ύX
	if (attackChara->myStatus->_yPos >= STAGE1_HEIGHT / 2) {
		drawOffset = -100;
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}
	else if (attackChara->myStatus->_yPos < STAGE1_HEIGHT / 2) {
		DrawRotaGraph(336, 240 + (int)drawOffset, 1.0f, 0.0f, FileManager::Instance()->GetFileHandle(DAMAGE_DETAIL), true);
	}

	// ���(�̗́A���O)�̕\���ʒu�̒�`
	float A_drawPosX = 390.0f, A_drawPosY = 260 + drawOffset;
	float D_drawPosX = 190.0f, D_drawPosY = 260 + drawOffset;

	// �U�����̏��̕`��
	DrawFormatString((int)A_drawPosX, (int)A_drawPosY - 50, GetColor(0, 0, 0), attackChara->myStatus->NAME.c_str());
	DrawFormatString((int)A_drawPosX - 30, (int)A_drawPosY, GetColor(0, 0, 0), "%d", attackChara->myStatus->HP);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + 100, A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(A_drawPosX, A_drawPosY,
		A_drawPosX + (100 * ((float)attackChara->myStatus->HP / (float)attackChara->myStatus->myParam.MaxHP)), A_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);

	// �h�䑤�̏��̕`��
	DrawFormatString((int)D_drawPosX, (int)D_drawPosY - 50, GetColor(0, 0, 0), defenceChara->myStatus->NAME.c_str());
	DrawFormatString((int)D_drawPosX - 30, (int)D_drawPosY, GetColor(0, 0, 0), "%d", defenceChara->myStatus->HP);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + 100, D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BARBOX), true);
	DrawExtendGraphF(D_drawPosX, D_drawPosY,
		D_drawPosX + (100 * ((float)defenceChara->myStatus->HP / (float)defenceChara->myStatus->myParam.MaxHP)), D_drawPosY + 15, FileManager::Instance()->GetFileHandle(HP_BAR), true);
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
		if (_character[num]->myStatus->xPos != x || _character[num]->myStatus->yPos != y) {
			_character[num]->OldPosX.clear();
			_character[num]->OldPosY.clear();
			_character[num]->moveCount = 0;
			isSelect = false;
		}
	}
}

void CharacterManager::CheckDetail(int x, int y)
{
	// ���ݏڍ׏���`�悵�Ă���Ȃ����
	if (isDetail) { 
		isDetail = false; 
		checkCharacter = nullptr; 
		return; 
	}

	// �J�[�\���������Ă���Ώڍ׏��`��
	for (size_t num = 0; num < _character.size(); num++) {
		if (_character[num]->myStatus->xPos == x && _character[num]->myStatus->yPos == y) {
			isDetail = true;
			checkCharacter = _character[num];
		}
	}
}

// �`�F�b�N���郆�j�b�g�̕ύX
void CharacterManager::ChangeDetailCharacter(shared_ptr<Character> const & character, int _index)
{
	// �C���f�b�N�X�ԍ�
	int index = 0;
	// �ŏ��Ƀ`�F�b�N�������j�b�g�̐w�c���������`�F�b�N
	if (character->myStatus->myTeam == "Player") {
		// �C���f�b�N�X�ԍ��̎擾
		for (int num = 0; num < _playerList.size(); ++num) {
			if (_playerList[num] == character) index = num;
		}
		// ���̗v�f��
		index += _index;
		// �v�f���𒴂��Ȃ��悤�ɒ���
		if (index == _playerList.size()) index = 0;
		else if (index < 0) index = (int)_playerList.size() - 1;
		checkCharacter = _playerList[index];
	}
	else if (character->myStatus->myTeam == "Enemy") {
		// �C���f�b�N�X�ԍ��̎擾
		for (int num = 0; num < _enemyList.size(); ++num) {
			if (_enemyList[num] == character) index = num;
		}
		// ���̗v�f��
		index += _index;
		// �v�f���𒴂��Ȃ��悤�ɒ���
		if (index == _enemyList.size()) index = 0;
		else if (index < 0) index = (int)_enemyList.size() - 1;
		checkCharacter = _enemyList[index];
	}
}

// �I������
void CharacterManager::Finalize() {
	if (_character.empty() == false) _character.clear();
	if (_myCharacter != nullptr) _myCharacter.reset();
	if (_eCharacter != nullptr) _eCharacter.reset();
	if (checkCharacter != nullptr) checkCharacter.reset();
}
