#ifndef PROJECTFILE_HPP
#define	PROJECTFILE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

namespace Nemrod {
    namespace MapTk {
        class ProjectFile {
        public:
            /**
             * Loads the given MapTK project file
             * 
             * @param fileName
             * @return 
             */
            static ProjectFile LoadProjectFile(std::string fileName);

            void SetImgs(std::vector<std::string> _imgs) {
                this->_imgs = _imgs;
            }

            const std::vector<std::string>& GetImgs() const {
                return _imgs;
            }

            void SetCopyright(std::string _copyright) {
                this->_copyright = _copyright;
            }

            std::string GetCopyright() const {
                return _copyright;
            }

            void SetProduct(std::string _product) {
                this->_product = _product;
            }

            std::string GetProduct() const {
                return _product;
            }
        private:
            std::string _product;
            std::string _copyright;
            std::vector<std::string> _imgs;
        };
    }
}
#endif	/* PROJECTFILE_HPP */

