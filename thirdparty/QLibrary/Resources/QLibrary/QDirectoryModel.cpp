#include <algorithm>
#include "QDirectoryModel.h"
#include <Engine/Core.h>
#include <engine/core/util/PathUtil.h>

namespace QT_UI
{
	// ���캯��
	QDirectoryModel::QDirectoryModel()
		: QStandardItemModel()
		, m_treeView(NULL)
		, m_activeItem(NULL)
		, m_proxy(NULL)
		, m_selectTime(0)
	{
		connect(this, &QDirectoryModel::signalThreadFindDirectory, this, &QDirectoryModel::OnThreadFindDirectory);
	}

	// ������Ŀ¼, �ļ����͹���
	void QDirectoryModel::SetRootPath(const char* rootPath, const char* extFilter, QTreeView* treeView, QSortFilterProxyModel* proxy)
	{
		updateRootPath(rootPath);

		// ������ʾ���ļ�����
		Echo::StringArray exts = Echo::StringUtil::Split(extFilter, "|");
		for (size_t i = 0; i<exts.size(); i++)
			m_exts.push_back(exts[i].c_str());

		m_treeView = treeView;
		m_proxy = proxy;
		connect(m_treeView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(OnExpandedFilter(const QModelIndex&)));
		connect(m_treeView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(OnExpandedFilter(const QModelIndex&)));
		connect(m_treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(OnSelectedFile(const QModelIndex&)));
		connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnEditFile(const QModelIndex&)));
	}

	void QDirectoryModel::updateRootPath(const char* rootPath)
	{
		m_rootPath = rootPath;
		Echo::PathUtil::FormatPathAbsolut(m_rootPath, true);
	}

	// �Ƿ�֧���ļ�����
	bool QDirectoryModel::IsSupportExt(const string& ext)
	{
		if (!m_exts.empty())
		{
			if (std::find(m_exts.begin(), m_exts.end(), ext) == m_exts.end())
				return false;
		}

		return true;
	}

	// ���
	void QDirectoryModel::Clean()
	{
		this->clear();
		m_dirItems.clear();
	}

	// ���õ�ǰѡ��
	void QDirectoryModel::setCurrentSelect(const char* dir)
	{
		// ����Item
		QStandardItem* lastItem = NULL;
		for (size_t i = 0; i<m_dirItems.size(); i++)
		{
			QStandardItem* item = m_dirItems[i];
			if (item && item->data(Qt::UserRole).toString() == dir)
			{
				lastItem = item;
				break;
			}
		}

		if (lastItem)
		{
			// ���õ�ǰѡ��
			QModelIndex lastIdx = indexFromItem(lastItem);
			m_treeView->setCurrentIndex(lastIdx);
			OnSelectedFile(lastIdx);

			// չ����
			QStandardItem* parentItem = lastItem->parent();
			while (parentItem)
			{
				QModelIndex parentIdx = indexFromItem(parentItem);
				m_treeView->expand(parentIdx);

				parentItem = parentItem->parent();
			}

			// չ������
			m_treeView->expand(lastIdx);
		}
	}

	// ˢ����ʾ
	void QDirectoryModel::Refresh(bool thread, bool volatile* interrupt)
	{
		if (Echo::PathUtil::IsDirExist(m_rootPath.c_str()))
		{
			if (NULL != interrupt && *interrupt)
				return;

			Echo::String pathName = m_rootPath.c_str();
			Echo::PathUtil::FormatPath(pathName);
			Echo::String dirName = Echo::PathUtil::GetLastDirName(pathName);
			QStandardItem* rootItem = new QStandardItem;
			rootItem->setText(dirName.c_str());
			rootItem->setIcon(m_iconMaps["root"]);
			rootItem->setData(m_rootPath.c_str(), Qt::UserRole);

			if (thread)
			{
				_addToRingQueue(invisibleRootItem(), rootItem);
			}
			else
				invisibleRootItem()->setChild(invisibleRootItem()->rowCount(), 0, rootItem);

			m_dirItems.push_back(rootItem);
			RecursiveDir(m_rootPath, rootItem, thread, interrupt);
		}
	}

	// ��ȡ�ļ�ͼ��
	QIcon QDirectoryModel::getFileIcon(const char* fullPath)
	{
		Echo::String fileExt = Echo::PathUtil::GetFileExt(fullPath, true);
		if (fileExt == ".png")
		{
			QPixmap pixmap(fullPath);
			return QIcon(pixmap.scaled(QSize(64, 64)));
		}

		return m_iconMaps[fileExt.c_str()];
	}

	// ����Ŀ¼
	void QDirectoryModel::RecursiveDir(const string& dir, QStandardItem* parentItem, bool thread, volatile bool* interrupt)
	{
		vector<QStandardItem*> dirItems;
		vector<QStandardItem*> fileItems;

		Echo::StringArray fileDirs;
		Echo::PathUtil::EnumFilesInDir(fileDirs, dir.c_str(), true, false, true);
		for (size_t i = 0; i<fileDirs.size(); i++)
		{
			if (NULL != interrupt && *interrupt)
				return;
			if (Echo::PathUtil::IsDir(fileDirs[i]))
			{
				Echo::PathUtil::FormatPathAbsolut(fileDirs[i], true);
				Echo::String dirName = Echo::PathUtil::GetLastDirName(fileDirs[i]);
				QStandardItem* childItem = new QStandardItem;
				childItem->setText(dirName.c_str());
				childItem->setIcon(m_iconMaps["filter"]);
				childItem->setData(fileDirs[i].c_str(), Qt::UserRole);
				if (thread)
				{
					_addToRingQueue(parentItem, childItem);
				}
				else
					dirItems.push_back(childItem);

				m_dirItems.push_back(childItem);
				RecursiveDir(fileDirs[i].c_str(), childItem, thread, interrupt);
			}
			else
			{
				Echo::String fileExt = Echo::PathUtil::GetFileExt(fileDirs[i], true);
				if (IsSupportExt(fileExt))
				{
					Echo::PathUtil::FormatPathAbsolut(fileDirs[i], true);
					Echo::String pureFileName = Echo::PathUtil::GetPureFilename(fileDirs[i]);
					QStandardItem* childItem = new QStandardItem;
					childItem->setText(pureFileName.c_str());
					childItem->setIcon(getFileIcon(fileDirs[i].c_str()));
					childItem->setData(fileDirs[i].c_str(), Qt::UserRole);

					if (!thread)
						fileItems.push_back(childItem);
				}
			}
		}

		int tRow = 0;

		// �Ȳ���Ŀ¼
		if (!thread)
		{
			for (size_t i = 0; i < dirItems.size(); i++)
			{
				parentItem->setChild(tRow, 0, dirItems[i]);
				tRow++;
			}

			// �����ļ�
			for (size_t i = 0; i < fileItems.size(); i++)
			{
				parentItem->setChild(tRow, 0, fileItems[i]);
				tRow++;
			}
		}
	}

	// չ��ĳFiter
	void QDirectoryModel::OnExpandedFilter(const QModelIndex& pIndex)
	{
		/*if( m_treeView->isExpanded( pIndex))
		itemFromIndex( pIndex)->setIcon( m_iconMaps["filterexpend"]);
		else
		itemFromIndex( pIndex)->setIcon( m_iconMaps["filter"]);*/
	}

	// �������λ�û�ȡĿ¼
	QString QDirectoryModel::getFileUnderMousePos(const QPoint& pos)
	{
		QModelIndex index = m_treeView->indexAt(pos);
		QString filePath = m_proxy ? m_proxy->data(index, Qt::UserRole).toString() : this->data(index, Qt::UserRole).toString();

		return filePath;
	}

	// ѡ��ĳ�ļ�
	void QDirectoryModel::OnSelectedFile(const QModelIndex& pIndex)
	{
		DWORD currenTime = GetTickCount();
		DWORD elapsedTime = currenTime - m_selectTime;
		m_selectTime = currenTime;

		if (m_currentSelect == pIndex)
			return;

		m_currentSelect = pIndex;
		QString filePath = m_proxy ? m_proxy->data(pIndex, Qt::UserRole).toString() : this->data(pIndex, Qt::UserRole).toString();

		// ȡ������ // ������ʾ
		if (m_activeItem)
			m_activeItem->setFont(m_treeView->font());

		// ���ü���
		m_activeItem = itemFromIndex(pIndex);
		if (m_activeItem)
		{
			// ������Ӧ
			QFont font = m_treeView->font();
			font.setBold(true);

			m_activeItem->setFont(font);
		}

		emit FileSelected(filePath.toStdString().c_str());
	}

	// ˫��ĳ�ļ�
	void QDirectoryModel::OnEditFile(const QModelIndex& pIndex)
	{
		QString filePath = m_proxy ? m_proxy->data(pIndex, Qt::UserRole).toString() : this->data(pIndex, Qt::UserRole).toString();

		// ȡ������ // ������ʾ
		if (m_activeItem)
			m_activeItem->setFont(m_treeView->font());

		// ���ü���
		m_activeItem = itemFromIndex(pIndex);
		if (m_activeItem)
		{
			// ������Ӧ
			QFont font = m_treeView->font();
			font.setBold(true);

			m_activeItem->setFont(font);
		}

		emit FileEdit(filePath.toStdString().c_str());
	}

	void QDirectoryModel::OnThreadFindDirectory()
	{
		//if (m_threadRingQueue.used() <= 0)
			return;
		RingQueueElem elem;
		//m_threadRingQueue.pop_front(elem);
		if (NULL != elem.parent && NULL != elem.child)
		{
			elem.parent->setChild(elem.parent->rowCount(), elem.child);
		}
	}

	void QDirectoryModel::ClearThreadRingQueue()
	{
		//m_threadRingQueue.clear_unsafe();
	}

	void QDirectoryModel::_addToRingQueue(QStandardItem* parent, QStandardItem* child)
	{
		//while (m_threadRingQueue.free() == 0)
		//	scl::sleep(100);
		//m_threadRingQueue.push_back(RingQueueElem(parent, child));
		//emit signalThreadFindDirectory();
	}



}