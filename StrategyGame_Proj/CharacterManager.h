#pragma once
#include "Character.h"

// �L�����N�^�[�̃f�[�^
#define UNIT_LOAD "Load.dat"
#define UNIT_SUPPORT "Support.dat"
#define UNIT_ENEMY1 "Enemy1.dat"
#define UNIT_ENEMY2 "Enemy2.dat"
#define DAMAGE_DETAIL "DamageDetail.png"
#define HP_BAR "HP_Bar.png"
#define HP_BARBOX "HP_BarBox.png"

class CharacterManager : public Singleton<CharacterManager>
{
public:
	~CharacterManager();

	// ��������L�����N�^�[
	vector<shared_ptr<Character>> _character;

	// �G(AI)�L�����N�^�[�̃��X�g
	vector<shared_ptr<Character>> _enemyList;
	// �v���C���[���̃L�����N�^�[�̃��X�g
	vector<shared_ptr<Character>> _playerList;

	// �U�����郆�j�b�g
	int attackUnitNum = 0;
	shared_ptr<Character> _myCharacter = nullptr;
	// �U������郆�j�b�g
	int blockUnitNum = 0;
	shared_ptr<Character> _eCharacter = nullptr;
	// �S�̂̍U����
	int attackCount = 0;

	// �^�[���ύX�A�j���[�V����
	bool turnAnim;
	// �v���C���[�̃^�[�����ǂ���
	bool playerTurn = false;
	// �I�𔻒�
	bool isSelect;
	// �ړ�����
	bool isMove = false;
	// �U������
	bool attack;
	// �Q�[�����I���������ǂ���
	bool isGame = true;

	// ������
	void Initialize();
	// �X�V
	void Update(int x, int y);
	// �^�[���J�n
	void StartTurn();

	// �`�悷�邩�`�F�b�N
	void DrawCheck(int x, int y);
	// �`��
	void Draw();

	// ���͌��m
	void KeyCheck(int x, int y);
	// ���j�b�g�̏ڍ׏�񌟍�
	bool isDetail;
	void CheckDetail(int x, int y);
	// ���݃`�F�b�N���̃��j�b�g
	shared_ptr<Character> checkCharacter = nullptr;
	// �`�F�b�N���郆�j�b�g�̕ύX
	void ChangeDetailCharacter(shared_ptr<Character> const &character, int _index);

	// �ړ��l�̎擾
	void GetMoveCount(int x, int y);
	// �ړ������擾
	void GetMoveArrow(int x, int y);
	// �ړ��G���A�̃��Z�b�g
	void MoveAreaClear();

	// �J�����Ƃ̃I�t�Z�b�g�v�Z
	void SetCameraOffset(int dir, bool horizontal);

	// �I������
	void Finalize();

private:

	// �v���C���[�f�[�^�̃p�X
	vector<string> playerDataPass;
	// �G�f�[�^�̃p�X
	vector<string> enemyDataPass;
	// ���j�b�g��I����Ԃɂ���
	void CanSelectCharacter(shared_ptr<Character> const &character, int x, int y);
	// �v���C���[�ƓG�̃f�[�^�̃p�X���i�[
	void SetCharacterData();
	// ���j�b�g���X�g�̃��Z�b�g
	void ResetCharacterList();
	// ���݂̓G(AI)�A�v���C���[�̃J�E���g
	void CharacterCount(shared_ptr<Character> const &character);

	// ���j�b�g�̈ړ�
	void CharacterMove(int x, int y);

	// �U���͈͕\��
	void GetAttackArea(int x, int y);
	// �U���ڍ׏��\��
	void DrawAttackDetail(shared_ptr<Character> const &character, int x, int y);
	// �U���̑Ώۂ�I��
	void ChoiseAttack(int x, int y);
	// �U���̏���
	void Attack();
	// �U�����̃f�[�^�\��
	void DrawDamageDetail(shared_ptr<Character> const &attackChara, shared_ptr<Character> const &defenceChara);

	// HP�p�摜
	int HpBar;
	int HpBarBox;
	int DamageDetail;
	// �s���\�ȃ��j�b�g��
	int moveableUnit = 0;
};