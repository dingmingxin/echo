#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <nodeeditor/NodeDataModel>
#include <iostream>
#include <string>
#include "DataFloat.h"
#include "QColorSelect.h"
#include "Compiler/ShaderCompiler.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace DataFlowProgramming
{
    /// The model dictates the number of inputs and outputs for the Node.
    /// In this example it has no logic.
    class ShaderDataModel : public NodeDataModel
    {
      Q_OBJECT

    public:
        ShaderDataModel();
        virtual ~ShaderDataModel() {}

		/// Caption is used in GUI
        virtual QString caption() const = 0;

        // variable name
        const Echo::String getVariableName();

        // generate code
        virtual bool generateCode(ShaderCompiler& compiler)=0;

        // show menu
        virtual void showMenu(const QPointF& pos) {}

	public:
		// number ports
		unsigned int nPorts(PortType portType) const override;

		// get port data type
		NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

		// get port data
		std::shared_ptr<NodeData> outData(PortIndex port) override;

    public:
		// validation
        virtual NodeValidationState validationState() const override { return m_modelValidationState; }
        virtual QString validationMessage() const override { return m_modelValidationError; }

        // check validation
        virtual bool checkValidation();

	protected:
        Echo::ui32                              m_id = 0;
        bool                                    m_isUsed = true;
        Echo::String                            m_variableName;

		std::vector<NodeDataType>               m_inputDataTypes;
		std::vector<std::shared_ptr<ShaderData>>m_inputs;
		vector<std::shared_ptr<ShaderData>>     m_outputs;

		NodeValidationState                     m_modelValidationState = NodeValidationState::Valid;
		QString                                 m_modelValidationError = QStringLiteral("");
    };
}

