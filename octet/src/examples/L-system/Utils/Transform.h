#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform
{
	octet::vec3 translation;
	octet::vec3 rotation;
	octet::vec3 scale;

public:
	Transform()
	{
		scale.x() = scale.y() = scale.z() = 1.0f;
	}
	~Transform(){}

	const octet::vec3& Translation() const { return translation; }
	octet::vec3& Translation() { return translation; }
	
	const octet::vec3& Rotation() const { return rotation; }
	octet::vec3& Rotation() { return rotation; }
	
	const octet::vec3& Scale() const { return scale; }
	octet::vec3& Scale() { return scale; }

	void SetIdentity()
	{
		translation.x() = translation.y() = translation.z() = 0.0f;
		rotation.x() = rotation.y() = rotation.z() = 0.0f;
		scale.x() = scale.y() = scale.z() = 1.0f;
	}

	octet::mat4t GetMatrix()
	{
		octet::mat4t matrix;
		matrix.loadIdentity();

		matrix.translate(translation);
		
		matrix.rotateX(rotation.x());
		matrix.rotateY(rotation.y());
		matrix.rotateZ(rotation.z());

		matrix.scale(scale.x(), scale.y(), scale.z());

		return matrix;
	}
};

#endif // !TRANSFORM_H
