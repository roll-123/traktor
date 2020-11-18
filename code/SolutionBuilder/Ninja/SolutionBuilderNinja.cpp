#include "Core/Io/FileOutputStream.h"
#include "Core/Io/FileSystem.h"
#include "Core/Io/IStream.h"
#include "Core/Io/Utf8Encoding.h"
#include "Core/Log/Log.h"
#include "Core/Misc/SafeDestroy.h"
#include "SolutionBuilder/Project.h"
#include "SolutionBuilder/ScriptProcessor.h"
#include "SolutionBuilder/Solution.h"
#include "SolutionBuilder/Ninja/SolutionBuilderNinja.h"

namespace traktor
{
	namespace sb
	{

T_IMPLEMENT_RTTI_CLASS(L"SolutionBuilderNinja", SolutionBuilderNinja, SolutionBuilder)

SolutionBuilderNinja::SolutionBuilderNinja()
{
}

SolutionBuilderNinja::~SolutionBuilderNinja()
{
	safeDestroy(m_scriptProcessor);
}

bool SolutionBuilderNinja::create(const CommandLine& cmdLine)
{
	if (cmdLine.hasOption(L"ninja-solution-template"))
		m_solutionTemplate = cmdLine.getOption(L"ninja-solution-template").getString();
	if (cmdLine.hasOption(L"ninja-project-template"))
		m_projectTemplate = cmdLine.getOption(L"ninja-project-template").getString();

	m_scriptProcessor = new ScriptProcessor();
	if (!m_scriptProcessor->create())
		return false;

	return true;
}

bool SolutionBuilderNinja::generate(Solution* solution)
{
	// Create root path.
	if (!FileSystem::getInstance().makeAllDirectories(solution->getRootPath()))
	{
		log::error << L"Unable to create output directory \"" << solution->getRootPath() << L"\"." << Endl;
		return false;
	}

	log::info << L"Generating Ninja projects..." << Endl;

	if (!m_scriptProcessor->prepare(m_projectTemplate))
	{
		log::error << L"Script processor failed using project template \"" << m_projectTemplate << L"\"." << Endl;
		return false;
	}

	for (auto project : solution->getProjects())
	{
		// Skip disabled projects.
		if (!project->getEnable())
			continue;

		std::wstring projectPath = solution->getRootPath() + L"/" + project->getName();

		if (!FileSystem::getInstance().makeDirectory(projectPath))
		{
			log::error << L"Unable to create project output directory \"" << projectPath << L"\"." << Endl;
			return false;
		}

		// Generate project
		{
			std::wstring projectOut;
			if (!m_scriptProcessor->generate(solution, project, projectPath, projectOut))
			{
				log::error << L"Script processor failed using project template \"" << m_projectTemplate << L"\"." << Endl;
				return false;
			}

			Ref< IStream > file = FileSystem::getInstance().open(
				projectPath + L"/" + project->getName() + L".ninja",
				File::FmWrite
			);
			if (!file)
			{
				log::error << L"Unable to create project file \"" << projectPath << L"/" << project->getName() << L".ninja" << L"\"." << Endl;
				return false;
			}

			FileOutputStream(file, new Utf8Encoding()) << projectOut;

			file->close();
		}
	}

	log::info << L"Generating Ninja solution..." << Endl;

	if (!m_scriptProcessor->prepare(m_solutionTemplate))
	{
		log::error << L"Script processor failed using solution template \"" << m_solutionTemplate << L"\"." << Endl;
		return false;
	}

	// Generate build file.
	{
		std::wstring solutionOut;
		if (!m_scriptProcessor->generate(solution, nullptr, solution->getRootPath(), solutionOut))
		{
			log::error << L"Script processor failed using solution template \"" << m_solutionTemplate << L"\"." << Endl;
			return false;
		}

		Ref< IStream > file = FileSystem::getInstance().open(
			solution->getRootPath() + L"/" + solution->getName() + L".ninja",
			File::FmWrite
		);
		if (!file)
		{
			log::error << L"Unable to create solution file \"" << solution->getRootPath() << L"/" + solution->getName() << L".ninja" << L"\"." << Endl;
			return false;
		}

		FileOutputStream(file, new Utf8Encoding()) << solutionOut;

		file->close();
	}

	return true;
}

void SolutionBuilderNinja::showOptions() const
{
	log::info << L"\t-ninja-solution-template=[template file]" << Endl;
	log::info << L"\t-ninja-project-template=[template file]" << Endl;
}

	}
}