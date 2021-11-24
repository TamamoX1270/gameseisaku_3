#pragma once

namespace nsK2Engine {
	enum EnSkyCubeType {
		enSkyCubeType_Day,		// ����
		enSkyCubeType_Night,	// ���
		enSkyCubeType_Snow,		// ��R
		enSkyCubeType_Snow_2,	// ��R_2
		enSkyCubeType_Wild,		// �r��
		enSkyCubeType_Wild_2,	// �r��Q
		enSkyCubeType_Wild_Night,	// �r��(���)
		enSkyCubeType_Grass,	// �Ő�
		enSkyCubeType_Euro,		// ���B
		enSkyCubeType_DayToon,	// ����(�g�D�[����)
		enSkyCubeType_DayToon_2,// ����(�g�D�[����)�Q
		enSkyCubeType_DayToon_3,// ����(�g�D�[����)�R
		enSkyCubeType_DayToon_4,// ����(�g�D�[����)�S
		enSkyCubeType_NightToon, // ���(�g�D�[����)
		enSkyCubeType_NightToon_2, // ���(�g�D�[����)�Q
		enSkyCubeType_SunriseToon,	// ������(�g�D�[����)
		enSkyCubeType_SpaceToon,	// ��C��(�g�D�[����)
		enSkyCubeType_SpaceToon_2,	// ��C��(�g�D�[����)�Q
		enSkyCubeType_Num,
	};
	/// <summary>
	/// ?
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start()override final;
		void Update()override final;
		void Render(RenderContext& rc)override final;
	public:
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}

		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		/// <summary>
		/// �X�J�C�L���[�u�̃^�C�v��ݒ�B
		/// </summary>
		/// <param name="type"></param>
		void SetType(EnSkyCubeType type) 
		{
			m_type = type;
		}
		/// <summary>
        /// ���邳��ݒ�B
        /// </summary>
        /// <param name="lum"></param>
		void SetLuminance(float lum)
		{
			m_luminance = lum;
		}

		//�ǉ������R�[�h��������
		void SetPosition2(const Vector3& pos2)
		{
			m_position2 = pos2;
			m_isDirty = true;
		}

		void SetScale2(const Vector3& scale2)
		{
			m_scale2 = scale2;
			m_isDirty = true;
		}
		void SetScale2(const float scale2)
		{
			m_scale2 = g_vec3One;
			m_scale2.Scale(scale2);
			m_isDirty = true;
		}
		/// <summary>
        /// �X�J�C�L���[�u�̃^�C�v��ݒ�2�B
        /// </summary>
        /// <param name="type"></param>
		void SetType2(EnSkyCubeType type2)
		{
			m_type2 = type2;
		}
		const wchar_t* GetTextureFilePath()
		{
			return m_textureFilePaths[m_type];
		}
		/// <summary>
        /// ���邳��ݒ�2�B
        /// </summary>
        /// <param name="lum"></param>
		void SetLuminance2(float lum)
		{
			m_luminance2 = lum;
		}
		/// <summary>
        /// ���E�̃X�e�[�g��ݒ�B
        /// </summary>
        /// <param name="state"></param>
		void SetWorldState(int state)
		{
			m_worldstate = state;
		}
		//�����܂�

	private:
		ModelRender m_modelRender;
		Texture m_texture[enSkyCubeType_Num];
		const wchar_t* m_textureFilePaths[enSkyCubeType_Num];
		Vector3 m_position = g_vec3Zero;
		Vector3 m_scale = g_vec3One * 1000.0f;
		float m_luminance = 3.8f;
		bool m_isDirty = false;
		EnSkyCubeType m_type = enSkyCubeType_Day;

		//�ǉ������R�[�h
		ModelRender m_modelRender2;
		int m_worldstate = 0;//�O����,�P����
		Vector3 m_position2 = g_vec3Zero;
		Vector3 m_scale2 = g_vec3One * 1000.0f;
		float m_luminance2 = 3.8f;
		EnSkyCubeType m_type2 = enSkyCubeType_Day;
	};
}

