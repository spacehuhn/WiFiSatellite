#ifndef SimpleSetting_Setting_h
#define SimpleSetting_Setting_h

#include <string>     // std::string
#include <sstream>    // std::stringstring
#include <functional> // std::function

namespace simplesetting {
    class Setting {
        public:
            virtual ~Setting() = default;

            virtual void set_on_change(std::function<void(Setting*)> on_change);

            virtual const std::string& get_name() const;
            virtual const std::string& get_comment() const;
            virtual std::string get_value() const = 0;

            virtual bool set_value(std::string value) = 0;

            virtual void next()  = 0;
            virtual void up()    = 0;
            virtual void down()  = 0;
            virtual void reset() = 0;

            virtual std::string to_ini() const;
            virtual std::string to_json_arr() const;
            virtual std::string to_json_obj() const;

            virtual bool equals(const Setting& setting) const;
            virtual bool equals(std::string name) const;

            bool operator==(const Setting& setting) const;
            bool operator==(std::string name) const;

        protected:
            std::string name { "" };
            std::string comment { "" };
            std::function<void(Setting*)> on_change { nullptr };

            void exec_on_change();
    };

    std::ostream& operator<<(std::ostream& os, const Setting& setting);
}
#endif /* ifndef SimpleSetting_Setting_h */