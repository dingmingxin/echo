#include "QPropertyDelegate.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRadioButton>
#include "QFileSelect.h"
#include "QCameraProperty.h"
#include "QAssetsSelect.h"
//#include "QVector3Editor.h"
#include "QColorSelect.h"
#include "QCheckBoxList.h"
#include "QUVEditor.h"
#include "Q2ndEditorButton.h"
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>

namespace QT_UI
{
	// ���캯��
	QPropertyDelegate::QPropertyDelegate( QPropertyModel* model, QObject *parent/* = 0*/)
		: QStyledItemDelegate(parent)
	{
		m_model = model;
	}

	// �Ƿ�ӵ���Զ�����Ⱦ
	bool QPropertyDelegate::IsSupportCustomPaint( const QString& widgetType) const
	{
		if( widgetType=="ColorSelect")
			return true;
		else if( widgetType=="CheckBox")
			return true;

		return false;
	}

	// �Զ�����Ⱦ
	void QPropertyDelegate::ItemDelegatePaint( QPainter *painter, const QString& widgetType, const QRect& rect, const QVariant& val) const
	{
		if (widgetType == "ColorSelect")
		{
			QColorSelect::ItemDelegatePaint(painter, rect, val.toString().toStdString());
		}
		else if( widgetType == "CheckBox")
		{
			QStyleOptionButton opt;
			opt.state |= val.toBool() ?  QStyle::State_On : QStyle::State_Off;
			opt.state |= QStyle::State_Enabled;
			opt.rect  =  QRect( rect.left()+3, rect.top(), rect.width()-3, rect.height());

			QApplication::style()->drawControl( QStyle::CE_CheckBox, &opt, painter);
		}
	}

	// ���ƺ���
	void QPropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		QVariant userData = index.model()->data( index, Qt::UserRole);
		QString  widgetType = userData.toString();

		QVariant value;
		if (m_model->findValue(value, propertyName) && IsSupportCustomPaint(widgetType))
		{
// 			if( IsSupportCustomPaint( widgetType))
// 			{
				ItemDelegatePaint( painter, widgetType, option.rect, value);
// 			}
// 			else
// 			{
// 				QString text = value.toString();
// 
// 				QStyleOptionViewItem myOption = option;
// 				myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
// 
// 				drawDisplay(painter, myOption, myOption.rect, text);
// 				drawFocus(painter, myOption, myOption.rect);
// 			}
		}
		else
		{
			QStyledItemDelegate::paint(painter, option, index);
		}
	}

	void QPropertyDelegate::commitEditor()
	{
		QWidget* ed = qobject_cast<QWidget*>(sender());
		if (ed)
		{
			emit commitData(ed);
		}
	}

	// createEditor, returns an editor widget
	QWidget* QPropertyDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = QString::fromUtf8( userData.toByteArray()).toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		
		QString propertyName = index.model()->data(index, Qt::DisplayPropertyRole).toString();
		// δָ���ؼ�,����NULL
		if( !userDatas.size())
			return NULL;

		QString  widgetType = userDatas[0].c_str();

		if (widgetType == "spinBox")
		{
			return new QSpinBox(parent);
		}
		else if (widgetType == "doubleSpinBox")
		{
			return new QDoubleSpinBox(parent);
		}
		else if (widgetType == "LineEdit")
		{
			return new QLineEdit(parent);
		}
		else if (widgetType == "FileSelect")
		{
			return new QFileSelect(parent);
		}
		else if (widgetType == "CameraPos")
		{
			return new QCameraPosDir(parent);
		}
		else if( widgetType == "AssetsSelect")
		{
			QAssetsSelect* widget = new QAssetsSelect( parent);
			if( widget && userDatas.size()>1)
			{
				widget->setExts( userDatas[1].c_str());

				if (userDatas.size() > 2)
					widget->setFiles(userDatas[2].c_str());
			}

			return widget;
		}
		else if (widgetType == "ColorSelect")
		{
			QColorSelect* widget = new QColorSelect(parent);
			QObject::connect(widget, SIGNAL(clicked()), this, SLOT(commitEditor()));
			return widget;
		}
		else if (widgetType == "RadioButton")
		{
			return new QRadioButton(parent);
		}
		else if( widgetType == "CheckBox")
		{
			QCheckBox* widget = new QCheckBox(parent);
			QObject::connect(widget, SIGNAL(stateChanged(int)), this, SLOT(commitEditor()));
			return widget;
		}
		else if (widgetType == "CheckBoxList")
		{
			return new QCheckBoxList(parent);
		}
		else if( widgetType == "ComboBox")
		{
			QComboBox* pWidget = new QComboBox( parent);
			for( size_t i=1; i<userDatas.size(); i++)
			{
				QString item = QString::fromUtf8(userDatas[i].c_str());
				pWidget->addItem(item);
			}
			return pWidget;
		}
		/*else if(widgetType == "Vector3")
		{
			return new QVector3EditorA(m_model, propertyName, parent);
		}
		else if (widgetType == "Vector4")
		{
			return new QVector4EditorA(m_model, propertyName, parent);
		}*/
		else if (widgetType == "UV")
		{
			return new QUVEditor(m_model, propertyName, parent);
		}
		else if (widgetType == "BoneSelectComboBox")
		{
			QComboBox* pWidget = new QComboBox(parent);
			m_model->ThrowSelfDefineSig("BoneSelectComboBoxEdit", propertyName, reinterpret_cast<qintptr>(pWidget));
			return pWidget;
		}
		else if (widgetType == "2ndEditor")
		{
			return new Q2ndEditor(m_model, parent); 
		}
		else if (widgetType == "default")
		{
			return QStyledItemDelegate::createEditor(parent, option, index);
		}

		return NULL;
	}

	// ���±༭������
	void  QPropertyDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		QString  widgetType = userDatas[0].c_str();

		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				widget->setRange(-65535, 65535);
				widget->setValue( value.toInt());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				widget->setRange(-65535.f, 65535.f); 
				widget->setValue( value.toFloat());
				widget->setSingleStep(0.1f); 
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				widget->setText( value.toString());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				widget->SetPath( value.toString());
			}
			else if (widgetType == "CameraPos")
			{
				QCameraPosDir* widget = qobject_cast<QCameraPosDir*>(editor);
				widget->SetPos(value.toString());
			}
			else if( widgetType == "AssetsSelect")
			{
				QAssetsSelect* widget = qobject_cast<QAssetsSelect*>(editor);
				widget->SetPath( value.toString());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>( editor);
				widget->SetColor( value.toString().toStdString());
				widget->OnSelectColor();
				m_model->setValue(propertyName, widget->GetColor().c_str());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				widget->setChecked( value.toBool());
				m_model->setValue(propertyName, widget->isChecked());
			}
			else if( widgetType == "CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				widget->SetValue( value.toString());
			}
			else if( widgetType == "ComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				widget->setCurrentText( value.toString());
			}
			/*else if(widgetType == "Vector3")
			{
				QVector3EditorA* widget = qobject_cast<QVector3EditorA*>(editor);
				widget->SetVector3(value.toString());
			}
			else if (widgetType == "Vector4")
			{
				QVector4EditorA* widget = qobject_cast<QVector4EditorA*>(editor);
				widget->SetVector4(value.toString());
			}*/
			else if (widgetType == "UV")
			{
				QUVEditor* widget = qobject_cast<QUVEditor*>(editor);
				widget->SetUV(value.toString());
			}
			else if (widgetType == "BoneSelectComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				if (value.toString().isEmpty())
				{
					widget->setCurrentIndex(-1);
				}
				widget->setCurrentText(value.toString());
			}
			else if (widgetType == "2ndEditor")
			{
				Q2ndEditor* widget = qobject_cast<Q2ndEditor*>(editor); 
				widget->setText(value.toString()); 
			}

			else if( widgetType == "default")
			{
				QStyledItemDelegate::setEditorData(editor, index);
			}
		}
	}

	// ����ģ������
	void  QPropertyDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		QVariant     userData		= index.model()->data( index, Qt::UserRole);
		Echo::String userData1	    = userData.toString().toStdString().c_str();
		Echo::StringArray userDatas = Echo::StringUtil::Split( userData1, ",");
		QString  widgetType = userDatas[0].c_str();
		
		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				this->m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				m_model->setValue( propertyName, widget->text());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				m_model->setValue( propertyName, widget->GetPath());
			}
			else if (widgetType == "CameraPos")
			{
				QCameraPosDir* widget = qobject_cast<QCameraPosDir*>(editor);
				m_model->setValue(propertyName, widget->GetPos());
			}
			else if( widgetType == "AssetsSelect")
			{
				QAssetsSelect* widget = qobject_cast<QAssetsSelect*>(editor);
				m_model->setValue( propertyName, widget->GetPath());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>(editor);
				m_model->setValue( propertyName, widget->GetColor().c_str());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				m_model->setValue( propertyName, widget->isChecked());
			}
			else if( widgetType=="CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				m_model->setValue( propertyName, widget->GetValue());
			}
			else if( widgetType=="ComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>( editor);
				m_model->setValue( propertyName, widget->currentText());
			}
			/*else if( widgetType=="Vector3")
			{
				QVector3EditorA* widget = qobject_cast<QVector3EditorA*>(editor);
				m_model->setValue(propertyName,widget->GetVector3());
			}
			else if (widgetType == "Vector4")
			{
				QVector4EditorA* widget = qobject_cast<QVector4EditorA*>(editor);
				m_model->setValue(propertyName, widget->GetVector4());
			}*/
			else if (widgetType == "UV")
			{
				//����������ⲿ�ֶ�����һ������ֵ��ʱ��,����model���ֵ��Ȼ���˵���û�м�ʱ���µ�����ϣ�
				//���µ��ô˺���ʱ����Ȼʹ��UI�ϵľ����ݸ����µġ�����ǿ�Ƶ���һ��setEditorData�����������ݵ�UI��
				setEditorData(editor, index);
				QUVEditor* widget = qobject_cast<QUVEditor*>(editor);
				m_model->setValue(propertyName, widget->GetUV());
			}
			else if (widgetType == "BoneSelectComboBox")
			{
				QComboBox* widget = qobject_cast<QComboBox*>(editor);
				m_model->setValue(propertyName, widget->currentText());
			}
			else if (widgetType == "2ndEditor")
			{
				Q2ndEditor* widget = qobject_cast<Q2ndEditor*>(editor);
				m_model->setValue(propertyName, widget->text());
			}

			else if( widgetType == "default")
			{
				QStyledItemDelegate::setModelData(editor, model, index);
			}
		}
	}

	// ���ñ༭��Geometry
	void QPropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const
	{
		QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
		if( widget)
		{
			QRect rect( option.rect.left()+3, option.rect.top(), option.rect.width()-3, option.rect.height());
			editor->setGeometry( rect);
			editor->setFocus();
		}
		else
		{
			editor->setGeometry(option.rect);
			editor->setFocus();
		}
	}

}