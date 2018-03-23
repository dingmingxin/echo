#ifndef _ECHO_PROJECTFILE_H_
#define _ECHO_PROJECTFILE_H_

#include "engine/core/Util/StringUtil.h"
#include "Rapidxml/rapidxml.hpp"
#include "Rapidxml/rapidxml_utils.hpp"
#include "Rapidxml/rapidxml_print.hpp"

using namespace rapidxml;

namespace Echo
{
	/**
	 * ��Ŀ�ļ�
	 */
	class ProjectFile
	{
	public:
		// �浵
		struct ArchiveItem
		{
			String		m_archiveType;		// �浵��Ŀ
			String		m_archiveValue;		// �浵ֵ

			ArchiveItem( const String& type, const String& value)
				: m_archiveType( type), m_archiveValue( value)
			{
				StringUtil::LowerCase(m_archiveType);
				StringUtil::LowerCase(m_archiveValue);
			}
		};

		enum TextureCompressType
		{
			DoNotCompress	= 0,
			DEFAULT			= 1,
			PVRTC1_2		= 2,
			PVRTC1_4		= 3,
			PVRTC1_2_RGB	= 4,
			PVRTC1_4_RGB	= 5,
			ETC1			= 6,  
			r4g4b4a4		= 7,
			r5g6b5			= 8,
		};

		// ����ѹ������
		struct TextureCompressItem
		{
			String				   m_name;			//��������
			TextureCompressType    m_iosCType;		//�Ƿ�ѹ����PVR����ʱ��bool���Ժ��ж���ѹ����ʽ�Ļ��ĳ�int��ö�٣�
			TextureCompressType    m_androidCType;  //�Ƿ�ѹ����ETC
			Real				   m_scale;			//ͼƬ��������

			TextureCompressItem(const String& name,TextureCompressType iosc = DEFAULT ,TextureCompressType androidc = DEFAULT,const Real& scale = -1.0f )
				: m_name(name)
				, m_iosCType(iosc)
				, m_androidCType(androidc)
				, m_scale(scale)
			{

			}

			TextureCompressItem()
				: m_name("")
				, m_iosCType(DEFAULT)
				, m_androidCType(DEFAULT)
				, m_scale(-1.0f)
			{

			}
		};
		typedef vector<TextureCompressItem>::type TextureCPIVec;

		struct RenderQueueInfo
		{
			String		m_name;
		};

	public:
		ProjectFile();
		~ProjectFile();

		// ��ȡ��Ŀ·��
		const String& getPath() const { return m_path; }

		// ��ȡ��Ŀ����
		const String& getName() const { return m_name; }

		// ��ȡȫ·��
		const String& getPathName() const { return m_pathName;  }

		// ����
		void load( const char* fileName);

		// ����
		void save( const char* fileName);

		// ��������
		void setupResource();

	public:
		// ��ȡ�浵��Ϣ
		vector<ArchiveItem>::type& getArchives() { return m_archives; }

		// ��ȡ����ѹ����Ϣ
		TextureCPIVec* getTextureCompreses(){ return &m_TextureCompreses; }

		//����ѹ����ʽ��Ӧ�ַ�������
		static char* getCompressTypeName(TextureCompressType ctype);

		//�����ַ������Ʒ���ѹ����ʽ��Ӧ
		static TextureCompressType getCompressTypeFormName(std::string cname);

		// ��ȡĬ�ϵĲ�������
		const String& getDefaultMaterial() { return m_defaultMaterial; }

	private:
		// ���ش浵����
		void loadArchives( xml_node<>* projectNode);

		// ����ui����
		void loadRenderQueues(xml_node<>* projectNode);

		// ����浵
		void saveArchives( xml_document<>& doc, xml_node<>* projectNode);

		// �Ƿ��Ѵ���
		bool isArchiveExist( const String& archiveType, const String& archiveValue);

		// ��ȡ����ѹ�������ļ�
		void loadTextureCompreses(xml_node<>* projectNode);

		// ��������ѹ�������ļ�
		void saveTextureCompreses(xml_document<>& doc, xml_node<>* projectNode);

	private:
		String								m_path;				// ��Ŀ·��
		String								m_name;				// ��Ŀ�ļ���
		String								m_pathName;			// ��Ŀȫ·��(��������)
		String								m_defaultMaterial;	// Ĭ�ϵĲ�������
		vector<ArchiveItem>::type			m_archives;			// �浵����
		vector<RenderQueueInfo>::type		m_renderQueues;		// ��Ⱦ������Ϣ
		TextureCPIVec						m_TextureCompreses; // ����ѹ����Ϣ��
	};
}

#endif