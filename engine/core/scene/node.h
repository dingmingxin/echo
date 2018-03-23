#pragma once

#include "engine/core/base/class.h"
#include <engine/core/Math/EchoMath.h>
#include "engine/core/Util/PtrMonitor.h"

namespace Echo
{
	class Node : public Object
	{
		ECHO_CLASS(Node, Object)

		friend class SceneManager;
		typedef set<Node*>::type ChildNodeSet;

	public:
		Node();
		virtual ~Node();

		ui32 getIdentifier() const { return m_identifier; }
		void setParent(Node* pParent);
		Node* getParent() const;

		ui32 getChildNum() const { return m_children.size(); }
		Node* getChild(ui32 idx);

		void setVisible(bool bVisible);
		bool isVisible() const;
		
		void scale(const Vector3& scl);
		void roll(const Real randian);
		void pitch(const Real randian);
		void yaw(const Real randian);
		void rotate(const Vector3& vAxis, const Real randian);
		void rotate(const Quaternion& rot);
		void translate(const Vector3& d);

		void setLocalScaling(const Vector3& scl);
		void setLocalScalingXYZ(Real x, Real y, Real z);
		void setLocalOrientation(const Quaternion& ort);
		void setLocalPosition(const Vector3& pos);
		void setLocalPositionXYZ(Real posX, Real posY, Real posZ);

		void setWorldOrientation(const Quaternion& ort);
		void setWorldPosition(const Vector3& pos);

		void update(float delta, bool bUpdateChildren = false);

		const Vector3& getLocalScaling() const;
		const Quaternion& getLocalOrientation() const;
		const Vector3& getLocalPosition() const;
		const Vector3& getWorldScaling() const;
		const Quaternion& getWorldOrientation() const;
		void getWorldOrientationWXYZ(Real* w = 0, Real* x = 0, Real* y = 0, Real* z = 0);
		const Vector3& getWorldPosition() const;
		void getWorldPositionXYZ(Real* x = 0, Real* y = 0, Real* z = 0);

		void setWorldMatrix(const Matrix4& tempMatrix);
		const Matrix4& getWorldMatrix();
		Matrix4* getWorldMatrixPtr();
		Matrix4	getInverseWorldMatrix() const;

		void convertWorldToLocalPosition(Vector3& posLocal, const Vector3& posWorld);
		void convertWorldToLocalOrientation(Quaternion& ortLocal, const Quaternion& ortWorld);

	public:
		Node* getNode(const String& path) { return nullptr; }

	protected:
		inline void		needUpdate();

		virtual void update() {}

	protected:
		ui32			m_identifier;		// Ψһ��ʶ��
		Node*			m_pParent;
		bool			m_bVisible;

		// only to notify the parent is modifyed.
		ChildNodeSet    m_children;

		Vector3			m_posLocal;
		Quaternion		m_ortLocal;
		Vector3			m_sclLocal;

		// Cached world space transform
		Vector3			m_posWorld;
		Quaternion		m_ortWorld;
		Vector3			m_sclWorld;

		/// Cached derived transform as a 4x4 matrix
		Matrix4			m_matWorld;

		bool			m_bModify;      //for caculate. ie: getWorldPostion
		bool			m_bMatrixDirty; //for rendering.
	};
}