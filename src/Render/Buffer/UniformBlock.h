//
// Created by KingSun on 2018/05/12
//

#ifndef KENGINE_UNIFORM_BLOCK_H
#define KENGINE_UNIFORM_BLOCK_H

#include <string>
#include <unordered_set>
#include <vector>
#include "../../KHeader.h"
#include "../Shader.h"

//Because for each uniform block is the same binding,
//so it's better to make the uniform to be static;
//share_ptr is a good choice.

namespace KEngine {
	namespace KBuffer {
		struct BlockData {
			BlockData(const char* name, const void* data):
				name(name), data(data){}
			const char* name;
			const void* data;
		};

		struct BlockMsg {
			BlockMsg(const char* name, Kint offset, Kint size):
				name(name), offset(offset), size(size){}
			const std::string name;
			Kint offset;
			Kint size;
		};

		class UnifromBlock {
		private:
			Kuint binding;
			Kuint ubo;
			Kuint program;
			const char* blockName;
			std::unordered_set<Kuint>* shaders;
			std::vector<BlockMsg*>* blockMsgs;

		public:
			UnifromBlock(const KRenderer::Shader* shader, const char* name) :
				program(shader->program), ubo(0), blockMsgs(nullptr), blockName(name),
				shaders(new std::unordered_set<Kuint>()) {
				//remember to relink program.
				shaders->emplace(program);
				Kuint index = glGetUniformBlockIndex(program, name);
				if (index == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform block name: " << blockName << "!" << std::endl;
					program = 0;
					return;
				}
				binding = index;
				glUniformBlockBinding(program, index, binding); //use it index as a binding point.
			}
			~UnifromBlock() {
				glDeleteBuffers(1, &ubo);
				delete shaders;
				if (blockMsgs == nullptr) return;
				for (auto &it : *blockMsgs) {
					delete it;
					it = nullptr;
				}
				delete blockMsgs;
			}

			void bindShader(const KRenderer::Shader* shader) {
				//remember to relink program.
				//remember to rebind shader when you want to prepare something in one shader.
				if (shaders->find(shader->program) != shaders->end()) {
					program = shader->program;
					return;
				}
				Kuint index = glGetUniformBlockIndex(shader->program, blockName);
				if (index == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform block name: " << blockName << "!" << std::endl;
					return;
				}
				program = shader->program;
				glUniformBlockBinding(program, index, binding);
			}

			void prepare(const std::vector<const char*>& names, Kboolean force = false) {
				if (!force && blockMsgs != nullptr) return;
				if (binding == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform blocks binding!" << std::endl;
					return;
				}
				Kint uboSize;
				glGetActiveUniformBlockiv(program, binding, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
				Ksize count = names.size();
				auto indices = new Kuint[count];
				glGetUniformIndices(program, count, names.data(), indices);

				auto offsets = new Kint[count];
				auto types = new Kint[count];
				auto sizes = new Kint[count];
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_OFFSET, offsets);
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_TYPE, types);
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_SIZE, sizes);

				if (blockMsgs == nullptr) {
					blockMsgs = new std::vector<BlockMsg*>();
					blockMsgs->reserve(count);

					glGenBuffers(1, &ubo);
					glBindBuffer(GL_UNIFORM_BUFFER, ubo);
					glBufferData(GL_UNIFORM_BUFFER, uboSize, nullptr, GL_DYNAMIC_DRAW);
					glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
				}
				else {
					glBindBuffer(GL_UNIFORM_BUFFER, ubo);
					//blockMsgs->reserve(blockMsgs->capacity() + count);
				}

				for (int i = 0; i < count; ++i) {
					blockMsgs->emplace_back(new BlockMsg(names[i], offsets[i],
						sizes[i] * getSize(types[i])));
					//std::cout << names[i] << ": " << offsets[i] << ", "
					//	<< sizes[i] << ", " << getSize(types[i]) << std::endl;
				}
				//glBindBuffer(GL_UNIFORM_BUFFER, 0);

				delete[] indices;
				delete[] offsets;
				delete[] types;
				delete[] sizes;
			}

			void prepare(const std::vector<BlockData>& blocks, Kboolean force = false) {
				if (!force && blockMsgs != nullptr) return;
				if (binding == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform blocks binding!" << std::endl;
					return;
				}
				Kint uboSize;
				glGetActiveUniformBlockiv(program, binding, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
				Ksize count = blocks.size();

				const auto names = new const Kchar*[count]; //do not delete it for it not a copy
				auto indices = new Kuint[count];
				for (int i = 0; i < count; ++i) {
					names[i] = blocks[i].name;
				}
				glGetUniformIndices(program, count, names, indices);

				auto offsets = new Kint[count];
				auto types = new Kint[count];
				auto sizes = new Kint[count];
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_OFFSET, offsets);
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_TYPE, types);
				glGetActiveUniformsiv(program, count, indices, GL_UNIFORM_SIZE, sizes);

				if (blockMsgs == nullptr) {
					blockMsgs = new std::vector<BlockMsg*>();
					blockMsgs->reserve(count);

					glGenBuffers(1, &ubo);
					glBindBuffer(GL_UNIFORM_BUFFER, ubo);
					glBufferData(GL_UNIFORM_BUFFER, uboSize, nullptr, GL_DYNAMIC_DRAW);
					glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
				}
				else {
					glBindBuffer(GL_UNIFORM_BUFFER, ubo);
					//blockMsgs->reserve(blockMsgs->capacity() + count);
					//note: when you use reserve, what it saved before will be undefined.
				}

				for (int i = 0; i < count; ++i) {
					blockMsgs->emplace_back(new BlockMsg(blocks[i].name, offsets[i],
						sizes[i] * getSize(types[i])));
					glBufferSubData(GL_UNIFORM_BUFFER, offsets[i],
						sizes[i] * getSize(types[i]), blocks[i].data);
					//std::cout << names[i] << ": " << offsets[i] << ", "
					//	<< sizes[i] << ", " << getSize(types[i]) << std::endl;
				}
				//glBindBuffer(GL_UNIFORM_BUFFER, 0);

				delete[] indices;
				delete[] offsets;
				delete[] types;
				delete[] sizes;
			}

			void allocate(const BlockData& data)const {
				if (binding == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform blocks binding!" << std::endl;
					return;
				}
				if (blockMsgs == nullptr) {
					std::cerr << "Please allocate blocks first!" << std::endl;
					return;
				}

				for (auto &it : *blockMsgs) {
					if (it->name == data.name) {
						glBindBuffer(GL_UNIFORM_BUFFER, ubo);
						glBufferSubData(GL_UNIFORM_BUFFER, it->offset, it->size, data.data);
						//glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
						glBindBuffer(GL_UNIFORM_BUFFER, 0);
						return;
					}
				}

				// glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				// Kchar* mapData = (Kchar*)glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
				// if (mapData != nullptr) memcpy(mapData + msg->offset, data.data, msg->size);
				// glUnmapBuffer(GL_UNIFORM_BUFFER);
			}

			void allocate(const std::vector<BlockData>& blocks)const {
				if (binding == GL_INVALID_INDEX) {
					std::cerr << "Wrong uniform blocks binding!" << std::endl;
					return;
				}
				if (blockMsgs == nullptr) {
					std::cerr << "Please allocate blocks first!" << std::endl;
					return;
				}

				glBindBuffer(GL_UNIFORM_BUFFER, ubo);

				for (auto &data : blocks) {
					for (auto &it : *blockMsgs) {
						if (it->name == data.name) {
							glBufferSubData(GL_UNIFORM_BUFFER, it->offset, it->size, data.data);
						}
					}
				}

				//glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		};
	}
}

#endif //KENGINE_UNIFORM_BLOCK_H
