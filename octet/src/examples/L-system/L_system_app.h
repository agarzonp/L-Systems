////////////////////////////////////////////////////////////////////////////////
//
//  Alejandro Garzon
//
// L-Sytem example using octet, a Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// L-Sytem example: simple 2D L-system
//
//

#include "LSystem/LSystem.h"
#include "CSVParser/CSVParser.h"

namespace octet {
 
  class L_system_app : public octet::app 
  {
	  enum State
	  {
		  SELECT_CONFIG,
		  MODIFYING_CONFIG,
		  RUNNING_CONFIG
	  };

	  State state;
	  State previousState;

	  int configIndex;

	  octet::dynarray<octet::string> configFiles;
	  octet::dynarray<LSystemConfig> lSystemConfigs;

	  LSystem lSystem;

    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

	// color shader
	color_shader color_shader_;

	// a texture for our text
    GLuint font_texture;

    // information for our text
    bitmap_font font;

    void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) 
	{
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 1);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      /*mat4t tmp;
      glLoadIdentity();
      glTranslatef(x, y, 0);
      glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);
      glScalef(scale, scale, 1);
      glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);*/

      enum { max_quads = 32 };
      bitmap_font::vertex vertices[max_quads*4];
      uint32_t indices[max_quads*6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      shader.render(modelToProjection, 0);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x );
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u );
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }

  public:

    // this is called when we construct the class
    L_system_app(int argc, char **argv) 
		: app(argc, argv)
		, font(512, 256, "assets/big.fnt")
		, state(SELECT_CONFIG)
		, previousState(SELECT_CONFIG)
		, configIndex(0)
	{
    }

    // this is called once OpenGL is initialized
    void app_init() 
	{
      // set up the shader
      texture_shader_.init();
	  color_shader_.init();

      // set up the matrices with a camera 5 units from the origin
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 0, 3);

      font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif"); 

	  // Load configurations
	  LoadConfigurations();
	}

	// this is called to simulate the world
	void simulate_world() 
	{
		switch (state)
		{
		case octet::L_system_app::SELECT_CONFIG:
			UpdateSelectConfig();
			break;

		case octet::L_system_app::MODIFYING_CONFIG:
			UpdateModifyingConfig();
			break;
		case octet::L_system_app::RUNNING_CONFIG:
			UpdateRunningConfig();
			break;
		default:
			break;
		}
	}

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) 
	{
      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  
	  // Draw LSystem
	  lSystem.Draw(color_shader_);

	  // Draw UI
	  DrawUI();
	  
    }
	
private:
	
	void LoadConfigurations()
	{
		// Parse the file that contains all LSystem configuration file names
		agarzonp::CSVParser csvParser("configFiles.csv");
		if (csvParser.IsValid())
		{
			for (int i = 0; i < csvParser.NumRows(); i++)
			{
				configFiles.push_back(csvParser[i][0]);
			}

		}
		lSystemConfigs.resize(configFiles.size());

		// load all config files
		LSystemConfigParser parser;
		for (unsigned i = 0; i < configFiles.size(); i++)
		{
			parser.LoadLSystemConfig(configFiles[i], lSystemConfigs[i]);
		}
	}

	void UpdateSelectConfig()
	{
		if (is_key_going_down(key_ctrl))
		{
			// modify current config
			state = MODIFYING_CONFIG;
			previousState = SELECT_CONFIG;
			modifyHotKey = MODIFY_NONE;
		}
		else if (is_key_going_down(key_esc))
		{
			// reset current config
			lSystemConfigs[configIndex].Reset();
		}
		else if (is_key_going_down(key_up) || is_key_going_down(key_right))
		{
			// next config
			configIndex++;
			if (configIndex >= (int)configFiles.size())
			{
				configIndex = 0;
			}
		}
		else if (is_key_going_down(key_down) || is_key_going_down(key_left))
		{
			// previous config
			configIndex--;
			if (configIndex < 0)
			{
				configIndex = configFiles.size() - 1;
			}
		}
		else if (is_key_going_down(key_space))
		{
			// execute the lsystem for the current config
			state = RUNNING_CONFIG;
			lSystem.Execute(lSystemConfigs[configIndex]);
		}
	}

	enum ModifyHotKey
	{
		MODIFY_NONE,

		MODIFY_F1,
		MODIFY_F2,
		MODIFY_F3,
	};

	ModifyHotKey modifyHotKey;

	void UpdateModifyingConfig()
	{
		if (is_key_going_down(key_ctrl))
		{
			// back to the previous state
			state = previousState;
		}
		else if (is_key_going_down(key_esc))
		{
			// reset current config
			lSystemConfigs[configIndex].Reset();
		}
		else if (is_key_going_down(key_f1))
		{
			modifyHotKey = MODIFY_F1;
		}
		else if (is_key_going_down(key_f2))
		{
			modifyHotKey = MODIFY_F2;
		}
		else if (is_key_going_down(key_f3))
		{
			modifyHotKey = MODIFY_F3;
		}
		else if (is_key_down(key_left) || is_key_down(key_down))
		{
			// decrease
			switch (modifyHotKey)
			{
			case octet::L_system_app::MODIFY_NONE:
				break;
			case octet::L_system_app::MODIFY_F1:
				lSystemConfigs[configIndex].n -= 1;
				break;
			case octet::L_system_app::MODIFY_F2:
				lSystemConfigs[configIndex].leftAngle -= 0.1f;
				break;
			case octet::L_system_app::MODIFY_F3:
				lSystemConfigs[configIndex].rightAngle -= 0.1f;
				break;
			default:
				break;
			}
		}
		else if (is_key_down(key_right) || is_key_down(key_up))
		{
			// increase
			switch (modifyHotKey)
			{
			case octet::L_system_app::MODIFY_NONE:
				break;
			case octet::L_system_app::MODIFY_F1:
				lSystemConfigs[configIndex].n += 1;
				break;
			case octet::L_system_app::MODIFY_F2:
				lSystemConfigs[configIndex].leftAngle += 0.1f;
				break;
			case octet::L_system_app::MODIFY_F3:
				lSystemConfigs[configIndex].rightAngle += 0.1f;
				break;
			default:
				break;
			}
		}
	}

	void UpdateRunningConfig()
	{
		if (is_key_going_down(key_ctrl))
		{
			// modify
			state = MODIFYING_CONFIG;
			previousState = RUNNING_CONFIG;
			modifyHotKey = MODIFY_NONE;
		}
		else if (is_key_going_down(key_esc))
		{
			// back to select config state
			state = SELECT_CONFIG;
			lSystem.Clear();
		}
		else if (is_key_going_down(key_up) || is_key_going_down(key_right))
		{
			lSystem.IncreaseIteration();
		}
		else if (is_key_going_down(key_down) || is_key_going_down(key_left))
		{
			lSystem.DecreaseIteration();
		}
	}

	void DrawUI()
	{
		// unbind the buffer, otherwise the UI disappears!!
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// draw number of config files
		char maxConfigFilesText[32];
		sprintf(maxConfigFilesText, "Max config files: %d\n", configFiles.size());
		draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, maxConfigFilesText);

		// Draw current config
		char currentConfigFileText[512];
		sprintf(currentConfigFileText, "Config file: %s\n", configFiles[configIndex].c_str());
		draw_text(texture_shader_, -1.75f, 1.5f, 1.0f / 256, currentConfigFileText);

		LSystemConfig& config = lSystemConfigs[configIndex];
		char currentConfigText[128];
		sprintf(currentConfigText, "n: %d", config.n);
		draw_text(texture_shader_, -1.75f, 0.5f, 1.0f / 256, currentConfigText);

		sprintf(currentConfigText, "leftAngle: %.1f", config.leftAngle);
		draw_text(texture_shader_, -1.75f, 0.25f, 1.0f / 256, currentConfigText);

		sprintf(currentConfigText, "rightAngle: %.1f", config.rightAngle);
		draw_text(texture_shader_, -1.75f, 0.0f, 1.0f / 256, currentConfigText);

		sprintf(currentConfigText, "Axiom: %s", config.axiom.c_str());
		draw_text(texture_shader_, -1.75f, -0.25f, 1.0f / 256, currentConfigText);


		std::string rules;
		for (auto itr = config.rules.begin(); itr != config.rules.end(); ++itr)
		{
			rules += itr->first;
			rules += " -> ";
			rules += itr->second;
			rules += "\n";
		}

		char rulesText[1024];
		sprintf(rulesText, "%s", rules.c_str());
		draw_text(texture_shader_, -1.75f, -0.5f, 1.0f / 256, rulesText);

		// Draw hot keys info
		char hotKeysInfo[64];
		switch (state)
		{
		case SELECT_CONFIG:

			sprintf(hotKeysInfo, "Arrow keys: ");
			draw_text(texture_shader_, -1.75f, -2.75f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change Config");
			draw_text(texture_shader_, -0.5f, -2.75f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Esc key: ");
			draw_text(texture_shader_, -1.75f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Reset Config");
			draw_text(texture_shader_, -0.5f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Ctrl key: ");
			draw_text(texture_shader_, -1.75f, -3.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change params");
			draw_text(texture_shader_, -0.5f, -3.25f, 1.0f / 256, hotKeysInfo);
			
			sprintf(hotKeysInfo, "Space key: ");
			draw_text(texture_shader_, -1.75f, -3.5f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Run LSystem");
			draw_text(texture_shader_, -0.5f, -3.5f, 1.0f / 256, hotKeysInfo);

			break;
		case MODIFYING_CONFIG:
			sprintf(hotKeysInfo, "F1 key: ");
			draw_text(texture_shader_, -1.75f, -2.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change n");
			draw_text(texture_shader_, -0.5f, -2.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "F2 key: ");
			draw_text(texture_shader_, -1.75f, -2.5f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change leftAngle");
			draw_text(texture_shader_, -0.5f, -2.5f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "F3 key: ");
			draw_text(texture_shader_, -1.75f, -2.75f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change righAngle");
			draw_text(texture_shader_, -0.5f, -2.75f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Arrow keys: ");
			draw_text(texture_shader_, -1.75f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Modify param");
			draw_text(texture_shader_, -0.5f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Ctrl key: ");
			draw_text(texture_shader_, -1.75f, -3.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Confirm changes");
			draw_text(texture_shader_, -0.5f, -3.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Esc key: ");
			draw_text(texture_shader_, -1.75f, -3.5f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Reset Config");
			draw_text(texture_shader_, -0.5f, -3.5f, 1.0f / 256, hotKeysInfo);

			break;
		case RUNNING_CONFIG:
			sprintf(hotKeysInfo, "Arrow keys: ");
			draw_text(texture_shader_, -1.75f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "(Un)Do iteration");
			draw_text(texture_shader_, -0.5f, -3.0f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Ctrl key: ");
			draw_text(texture_shader_, -1.75f, -3.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Change params");
			draw_text(texture_shader_, -0.5f, -3.25f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Esc key: ");
			draw_text(texture_shader_, -1.75f, -3.5f, 1.0f / 256, hotKeysInfo);

			sprintf(hotKeysInfo, "Select config");
			draw_text(texture_shader_, -0.5f, -3.5f, 1.0f / 256, hotKeysInfo);
			break;
		default:
			break;
		}
	}

  };
}
