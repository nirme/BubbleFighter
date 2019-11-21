#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"
#include "Matrix3.h"
#include "Vector2.h"
#include "Vector3.h"

#include "_2d/Renderable.h"
#include "ShadingProgram.h"
#include "ShadingParamsPassthru.h"



namespace core
{
	enum UNIFORM_USAGE
	{
		UU_UNKNOWN = 0x00,

		UU_2D_WORLD_MATRIX = 0x01,
		UU_2D_VIEW_MATRIX = 0x02,
		UU_2D_WORLDVIEW_MATRIX = 0x03,

		UU_AMBIENT_LIGHT_COLOUR = 0x04,

		UU_CUSTOM = 0x05,
		UU_TIME = 0x06,

		//		UU_LIGHT_POSITION = 0x07,
	};

	struct Uniform
	{
		UNIFORM_USAGE usage;
		std::string name;
		GLint id;
		GLint size;
		GLenum type;

		Uniform(UNIFORM_USAGE _usage, const std::string &_name, GLint _id, GLint _size, GLenum _type) :
			usage(_usage), 
			name(_name), 
			id(_id), 
			size(_size), 
			type(_type)
		{};
	};


	class ShadingProgramParams
	{
	protected:
		ShadingProgramPtr program;

		typedef std::vector<Uniform> UniformList;

		UniformList programUniforms;


	public:

		void addUniformDefinition(const std::string &_name, GLint _id, GLint _size, GLenum _type)
		{
			programUniforms.emplace_back(getUsage(_name), _name, _id, _size, _type);
		};


		void applyUniformValues(ShadingParamsPassthru* paramsps) const
		{
			for (auto it = programUniforms.begin(); it != programUniforms.end(); ++it)
			{
				switch ((*it).usage)
				{
				case UU_2D_WORLD_MATRIX:
				{
					GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dWorldMatrix().m));
					break;
				}

				case UU_2D_VIEW_MATRIX:
				{
					GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dViewMatrix().m));
					break;
				}

				case UU_2D_WORLDVIEW_MATRIX:
				{
					GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dWorldViewMatrix().m));
					break;
				}

				case UU_AMBIENT_LIGHT_COLOUR:
				{
					GL_ERROR_CHECK(glUniform3fv((*it).id, 1, paramsps->getAmbientLight().v));
					break;
				}

				case UU_TIME:
				{
					GL_ERROR_CHECK(glUniform1f((*it).id, paramsps->getTimeElapsed()));
					break;
				}

				case UU_CUSTOM:
				{
					// add custom params
					break;
				}

				default:
					break;

				}
			}
		};



	protected:
		typedef std::unordered_map<std::string, UNIFORM_USAGE> UniformNameMap;
		static const UniformNameMap namedUniformList;

	public:
		static UNIFORM_USAGE getUsage(const std::string &_name)
		{
			auto it = namedUniformList.find(_name);
			return it != namedUniformList.end() ? (*it).second : UNIFORM_USAGE::UU_CUSTOM;
		};


	};

	const ShadingProgramParams::UniformNameMap ShadingProgramParams::namedUniformList = ShadingProgramParams::UniformNameMap(
		{
			{ "worldMx", UNIFORM_USAGE::UU_2D_WORLD_MATRIX },
			{ "viewMx", UNIFORM_USAGE::UU_2D_VIEW_MATRIX},
			{ "worlViewdMx", UNIFORM_USAGE::UU_2D_WORLDVIEW_MATRIX},
			{ "ambientColor", UNIFORM_USAGE::UU_AMBIENT_LIGHT_COLOUR},
			{ "time", UNIFORM_USAGE::UU_TIME}
		});

}
